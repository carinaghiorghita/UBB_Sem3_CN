#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    struct sockaddr_in server;
    char a[255],b[255],res[255];

    int s=socket(AF_INET,SOCK_STREAM,0);
    if(s<0){
        perror("socket");
        exit(1);
    }

    memset(&server,0,sizeof(server));
    server.sin_addr.s_addr=inet_addr("192.168.1.7");
    server.sin_port=htons(1500);
    server.sin_family=AF_INET;

    if(connect(s,(struct sockaddr*)&server, sizeof(server))<0){
        perror("connect");
        exit(1);
    }

    printf("a:");
    fgets(a,255,stdin);
    printf("b:");
    fgets(b,255,stdin);

    send(s,a,255,0);
    send(s,b,255,0);

    recv(s,res,255,0);
    printf("merged strings:%s",res);

    close(s);
    return 0;
}
