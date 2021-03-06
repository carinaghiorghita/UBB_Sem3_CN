#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    struct sockaddr_in server,client;

    int s=socket(AF_INET,SOCK_DGRAM,0);

    bzero(&server,sizeof(server));

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(1500);

    bind(s,(struct sockaddr*)&server,sizeof(server));

    int clLen=sizeof(struct sockaddr_in);

    while(1){
        char str[255],ch;
        uint16_t pos[255],n=0;

        recvfrom(s,str,255,0,(struct sockaddr*)&client,(socklen_t *)&clLen);
        recvfrom(s,&ch,sizeof(ch),0,(struct sockaddr*)&client,(socklen_t *)&clLen);

        for(int i=0;str[i]!='\0';++i){
            if(str[i]==ch) pos[n++]=i;
        }

        int N=n;
        n=htons(n);
        sendto(s,&n,sizeof(n),0,(struct sockaddr*)&client,clLen);

        for(int i=0;i<N;++i){
            pos[i]=htons(pos[i]);
            sendto(s,&pos[i],sizeof(pos[i]),0,(struct sockaddr*)&client,clLen);
        }
    }
    return 0;
}
