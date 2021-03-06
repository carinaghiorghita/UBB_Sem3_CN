#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int s,c,res,len;
    struct sockaddr_in server,client;

    s=socket(AF_INET,SOCK_STREAM,0);
    if(s<0){
        perror("socket");
        exit(1);
    }

    server.sin_port=htons(1500);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;

    if(bind(s,(struct sockaddr*)&server,sizeof(server))<0){
        perror("bind");
        exit(1);
    }

    listen(s,5);
    len=sizeof(server);
    memset(&client,0,len);

    while(1){
        char str[255],ch;
        uint16_t nr=0,pos[255];

        printf("Waiting for data on port TCP 1500\n");

        c=accept(s,(struct sockaddr*)&server,&len);
        if(c<0){
            perror("accept");
            exit(1);
        }

        res=recv(c,&str,255,0);
        if(res<0){
            perror("recv");
            exit(1);
        }

        res=recv(c,&ch,1,0);
        if(res<0){
            perror("recv");
            exit(1);
        }

        for(int i=0;str[i]!='\0';++i){
            if(str[i]==ch){
                pos[nr]=i;
                pos[nr]=htons(pos[nr]);
                nr++;
            }
        }
        int N=nr;
        nr=htons(nr);
        res=send(c,&nr,sizeof(nr),0);
        if(res<0){
            perror("send");
            exit(1);
        }

        res=send(c,&pos,N*sizeof(pos),0);
        if(res<0){
            perror("send");
            exit(1);
        }

        close(c);
    }

    return 0;
}
