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
    int s;
    struct sockaddr_in server,client;
    char msg[255];

    s=socket(AF_INET,SOCK_STREAM,0);
    if(s<0){
        printf("Error creating socket");
        return -1;
    }

    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_family=AF_INET;
    server.sin_port=htons(1500);

    if(bind(s,(struct sockaddr*)&server,sizeof(server))<0){
        printf("Error binding");
        return -1;
    }

    listen(s,5);

    int len=sizeof(server);
    memset(&client,0,len);

    while(1){
        printf("Waiting for data on port TCP 1500\n");

        int c=accept(s,(struct sockaddr*)&client,&len);
        if(c<0){
            printf("Error accepting connection");
            return -1;
        }

        int res=recv(c,&msg,256,0);
        if(res<0){
            printf("Error receiving data");
            return -1;
        }

        int msgLen=strlen(msg);
        for(int i=0;i<msgLen/2;++i){
            char ch=msg[i];
            msg[i]=msg[msgLen-i-1];
            msg[msgLen-i-1]=ch;
        }
        msg[msgLen]='\0';

        res=send(c,&msg,msgLen+1,0);
        if(res<0){
            printf("Error sending data");
            return -1;
        }

        close(c);
    }

    return 0;
}
