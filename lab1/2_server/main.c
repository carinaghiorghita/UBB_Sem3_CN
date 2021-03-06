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

    server.sin_port=htons(1500);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(s,(struct sockaddr*)&server,sizeof(server))<0){
        printf("Error binding");
        return -1;
    }

    listen(s,5);

    int len=sizeof(server);
    memset(&client,0,len);

    while(1){
        printf("Waiting for data on port TCP 1500\n");
        uint16_t count=0;
        int c=accept(s,(struct sockaddr*)&client,&len);
        if(c<0){
            printf("Error accepting connection");
            return -1;
        }

        int res=recv(c,&msg,256,0);
        if(res<0){
            printf("Error receiving message");
            return -1;
        }

        for(int i=0;msg[i]!='\0';++i)
            if(msg[i]==' ') count++;
        count=htons(count);
        res=send(c,&count,sizeof(count),0);
        if(res<0){
            printf("Error sending");
            return -1;
        }
        close(c);
    }

    return 0;
}
