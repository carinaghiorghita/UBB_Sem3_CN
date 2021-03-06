#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    struct sockaddr_in server;
    char str[255],ch;
    uint16_t n,x;

    int s=socket(AF_INET,SOCK_DGRAM,0);

    server.sin_port=htons(1500);
    server.sin_addr.s_addr=inet_addr("192.168.1.8");
    server.sin_family=AF_INET;

    printf("Your string:");
    fgets(str,255,stdin);

    printf("Char:");
    scanf("%c",&ch);

    int len= sizeof(server);

    sendto(s,str,256,0,(struct sockaddr*)&server,len);
    sendto(s,&ch, sizeof(ch),0,(struct sockaddr*)&server,len);

    recvfrom(s,&n, sizeof(n),0,(struct sockaddr*)&server,(socklen_t *)&len);
    n=ntohs(n);

    printf("Pos:");
    for(int i=0;i<n;++i){
        recvfrom(s,&x, sizeof(x),0,(struct sockaddr*)&server,(socklen_t *)&len);
        x=ntohs(x);
        printf("%hu ",x);
    }
    return 0;
}
