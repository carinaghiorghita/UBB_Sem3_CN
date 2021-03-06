#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    struct sockaddr_in server;
    uint16_t n,x,sum;

    int s=socket(AF_INET,SOCK_DGRAM,0);
    if(s<0){perror("socket");exit(1);}

    server.sin_addr.s_addr=inet_addr("192.168.1.8");
    server.sin_port=htons(1500);
    server.sin_family=AF_INET;

    int serverLen= sizeof(struct sockaddr_in);

    printf("n=");
    scanf("%hu",&n);

    int N=n;
    N=htons(N);
    sendto(s,&N, sizeof(N),0,(struct sockaddr*)&server,serverLen);

    for(int i=0;i<n;++i){
        printf("arr[%d]=",i);
        scanf("%hu",&x);
        x=htons(x);
        sendto(s,&x, sizeof(x),0,(struct sockaddr*)&server,serverLen);
    }

    recvfrom(s,&sum, sizeof(sum),0,(struct sockaddr*)&server,&serverLen);
    sum=ntohs(sum);

    printf("Sum=%hu",sum);
    return 0;
}
