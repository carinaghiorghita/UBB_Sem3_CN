#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    struct sockaddr_in server;
    uint16_t n,k,arr[100];

    int s=socket(AF_INET, SOCK_STREAM,0);
    if(s<0){
        perror("socket");
        exit(1);
    }

    memset(&server,0, sizeof(server));
    server.sin_port=htons(1500);
    server.sin_addr.s_addr=inet_addr("192.168.1.7");
    server.sin_family=AF_INET;

    if(connect(s,(struct sockaddr*)&server,sizeof(server))<0){
        perror("connect");
        exit(1);
    }

    printf("n=");
    scanf("%hu",&n);
    n=htons(n);
    if(send(s,&n,sizeof(n),0)<0){
        perror("Error sending");
        exit(1);
    }

    recv(s,&k, sizeof(k),0);
    k=ntohs(k);

    printf("Divisors:");
    recv(s,&arr,k* sizeof(uint16_t),0);
    for(int i=0;i<k;++i){
        arr[i]=ntohs(arr[i]);
        printf("%hu ",arr[i]);
    }

    close(s);
    return 0;
}
