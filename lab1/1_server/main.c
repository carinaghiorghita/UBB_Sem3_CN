#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 1500
#define MAX_MSG 100

int main(int argc, char* argv[]) {
    struct sockaddr_in clientAddr, servAddr;
    uint16_t sum,n;

    int sd= socket(AF_INET, SOCK_STREAM, 0);

    if (sd < 0) {
        printf("Cannot open socket");
        return -1;
    }

    //bind server port
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);

    if (bind(sd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        printf("Cannot bind port");
        return -1;
    }

    int clientLen = sizeof(servAddr);
    memset(&clientAddr,0,clientLen);
    listen(sd, 5);
    while (1) {
        printf("%s: Waiting for data on port TCP %u\n",argv[0],SERVER_PORT);

        int c = accept(sd,(struct sockaddr*)&clientAddr,&clientLen);

        if (c < 0) {
            printf("Cannot accept connection");
            return -1;
        }
        int res=recv(c,&n,sizeof(n),0);
        if(res<0){
            printf("Error receiving");
            return -1;
        }
        n=ntohs(n);
        uint16_t arr[n];

        res=recv(c,&arr,n*sizeof(uint16_t),0);
        if(res<0){
            printf("Error receiving");
            return -1;
        }
        sum=0;
        for(int i=0;i<n;++i){
            arr[i]=ntohs(arr[i]);
            sum+=arr[i];
        }
        sum=htons(sum);
        res=send(c,&sum,sizeof(sum),0);
        if(res<0){
            printf("Error sending");
            return -1;
        }
        close(c);
    }

    return 0;
}
