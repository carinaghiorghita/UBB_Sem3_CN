
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
    len=sizeof(server);
    memset(&client,0,len);

    while(1){
        uint16_t n,count=0,div[100];
        printf("Waiting for data on port TCP 1500\n");

        c=accept(s,(struct sockaddr*)&client,&len);
        if(c<0){
            printf("Error accepting connection");
            return -1;
        }

        res=recv(c,&n,sizeof(n),0);
        n=ntohs(n);
        if(res<0){
            printf("Error receiving data");
            return -1;
        }

        for(int i=1;i*i<=n;++i)
            if(n%i==0){
                div[count]=i;
                div[count]=htons(div[count]);
                count++;
                div[count]=n/i;
                div[count]=htons(div[count]);
                count++;
            }
        count=htons(count);
        res=send(c,&count,sizeof(count),0);
        if(res<0){
            printf("Error sending data");
            return -1;
        }

        res=send(c,&div,count*sizeof(uint16_t),0);
        if(res<0){
            printf("Error sending data");
            return -1;
        }

        close(c);
    }
    return 0;
}
