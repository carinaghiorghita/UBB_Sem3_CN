#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

int main() {
    struct sockaddr_in broadcast;
    char msg[255];
    int yes=1;

    int s=socket(PF_INET,SOCK_DGRAM,0);

    setsockopt(s,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(int));

    memset(&broadcast,0,sizeof(broadcast));

    broadcast.sin_port=htons(1500);
    broadcast.sin_addr.s_addr=INADDR_ANY;
    broadcast.sin_family=AF_INET;

    int brLen=sizeof(broadcast);
    bind(s,(struct sockaddr*)&broadcast,brLen);

    recvfrom(s,msg,255,0,(struct sockaddr*)&broadcast,&brLen);

    printf("Received: %s",msg);
    return 0;
}
