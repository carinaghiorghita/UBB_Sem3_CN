#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    struct sockaddr_in broadcast;
    int yes=1;
    char msg[255];

    int s=socket(PF_INET,SOCK_DGRAM,0);
    if(s<0){
        perror("socket");
        exit(1);
    }

    if(setsockopt(s,SOL_SOCKET,SO_BROADCAST,&yes, sizeof(int))<0){
        perror("setting broadcast option");
        exit(1);
    }

    memset(&broadcast,0, sizeof(broadcast));
    broadcast.sin_addr.s_addr=inet_addr("192.168.11.255");
    broadcast.sin_port=htons(1500);
    broadcast.sin_family=AF_INET;

    printf("Message to send:");
    fgets(msg,255,stdin);

    while(1){
        sendto(s,msg,256,0,(struct sockaddr*)&broadcast, sizeof(broadcast));
        sleep(3);
    }
    return 0;
}
