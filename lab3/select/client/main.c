#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

fd_set master,read_fds;
int sock;
struct sockaddr_in server;
char buff[256];
int nbytes,port,ipaddr;

int main(int argc,char** argv) {
    if(argc<3){
        printf("Usage: <hostname or IP address> <port>\n");
        exit(1);
    }

    port=atoi(argv[2]);

    ipaddr=inet_addr(argv[1]);
    if(ipaddr<0){
        struct in_addr inaddr;
        struct hostent* host=gethostbyname(argv[1]);
        if(host==NULL){
            perror("get host address");
            exit(1);
        }

        memcpy(&inaddr.s_addr,host->h_addr_list[0], sizeof(inaddr));
        printf("Connecting to %s...\n",inet_ntoa(inaddr));
        memcpy(&ipaddr,host->h_addr_list[0], sizeof(unsigned long int));
    }

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        exit(1);
    }

    memset(&server,0, sizeof(server));
    server.sin_port=htons(port);
    server.sin_addr.s_addr=ipaddr;
    server.sin_family=AF_INET;

    if(connect(sock,(struct sockaddr*)&server, sizeof(server))<0){
        perror("connect");
        exit(1);
    }

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(0,&master);
    FD_SET(sock,&master);

    while(1){
        read_fds=master;

        if(select(sock+1,&read_fds,NULL,NULL,NULL)<0){
            perror("select");
            exit(1);
        }

        if(FD_ISSET(0,&read_fds)){
            nbytes=read(0,buff, sizeof(buff)-1);
            int ret=send(sock,buff,nbytes,0);
            if(ret<=0){
                perror("send");
                exit(1);
            }
        }

        if(FD_ISSET(sock,&read_fds)){
            nbytes=read(sock,buff,sizeof(buff)-1);
            if(nbytes<=0){
                printf("Server has closed connection... closing...\n");
                exit(1);
            }
            write(1,buff,nbytes);
        }
    }
    return 0;
}
