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
        char str[255],substr[255];
        uint16_t lens,ind,count=0;

        printf("Waiting for data on port TCP 1500\n");

        c=accept(s,(struct sockaddr*)&server,(socklen_t *)&len);
        if(c<0){
            perror("accept");
            exit(1);
        }

        recv(c,&str,256,0);

        recv(c,&ind,sizeof(ind),0);
        ind=ntohs(ind);

        recv(c,&lens,sizeof(lens),0);
        lens=ntohs(lens);

        for(int i=ind;i<ind+len;++i){
            substr[i-ind]=str[i];
        }
        substr[ind+len]='\0';

        send(c,&substr,sizeof(substr),0);

        close(c);
    }

    return 0;
}
