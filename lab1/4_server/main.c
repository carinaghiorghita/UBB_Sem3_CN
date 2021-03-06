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
    char a[255],b[255],msg[255];

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
        return-1;
    }

    listen(s,5);

    int len=sizeof(server);
    memset(&client,0,len);

    while(1){
        printf("Waiting for data on port TCP 1500\n");

        int c=accept(s,(struct sockaddr*)&client,&len);

        if(c<0){
            printf("Error connecting");
            return -1;
        }

        int res=recv(c,&a,256,0);
        if(res<0){
            printf("Error receiving data");
            return -1;
        }
        res=recv(c,&b,256,0);
        if(res<0){
            printf("Error receiving data");
            return -1;
        }

        int i=0,j=0,la=strlen(a),lb=strlen(b),k=0;
        while(i<la&&j<lb){
            if(a[i]<b[j])msg[k++]=a[i++];
            else msg[k++]=b[j++];
        }
        while(i<la)msg[k++]=a[i++];
        while(j<lb)msg[k++]=b[j++];

        msg[k]='\0';

        res=send(c,&msg,sizeof(msg),0);
        if(res<0){
            printf("Error sending data");
            return -1;
        }

        close(c);
    }

    return 0;
}
