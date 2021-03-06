#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

int main() {
    struct sockaddr_in server,client;

    int s=socket(AF_INET,SOCK_DGRAM,0);
    if(s<0){perror("socket");exit(1);}

    bzero(&server,sizeof(server));

    server.sin_port=htons(1500);
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_family=AF_INET;

    if(bind(s,(struct sockaddr*)&server,sizeof(server))<0){
        perror("bind");
        exit(1);
    }

    int clientLen=sizeof(struct sockaddr_in);
    while(1){
        uint16_t n,sum=0,x;

        recvfrom(s,&n,sizeof(n),0,(struct sockaddr*)&client,&clientLen);
        n=ntohs(n);

        for(int i=0;i<n;++i){
            recvfrom(s,&x,sizeof(x),0,(struct sockaddr*)&client,&clientLen);
            x=ntohs(x);
            sum+=x;
        }

        sum=htons(sum);
        sendto(s,&sum,sizeof(sum),0,(struct sockaddr*)&client,clientLen);

    }
    return 0;
}
