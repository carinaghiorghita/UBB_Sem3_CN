#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

fd_set master,read_fds;
int sock;
struct sockaddr_in server;
char buff[256];
int nbytes,port,ipaddr;

int main(int argc,char** argv) {


    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        exit(1);
    }

    memset(&server,0, sizeof(server));
    server.sin_port=htons(7000);
    server.sin_addr.s_addr=inet_addr("192.168.75.128");
    server.sin_family=AF_INET;

    if(connect(sock,(struct sockaddr*)&server, sizeof(server))<0){
        perror("connect");
        exit(1);
    }

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(0,&master);
    FD_SET(sock,&master);

    uint16_t nrDigits,pos,countAll=0,nr;
    bool res=false;
    res=htons(res);

    if(recv(sock,&nrDigits, sizeof(nrDigits),0)<0){
        perror("recv");
        exit(1);
    }
    nrDigits=ntohs(nrDigits);
    printf("%d\n",nrDigits);

    uint16_t allDigits[nrDigits];
    for(int i=0;i<nrDigits;++i)allDigits[i]=-1;

    while(1){
        read_fds=master;
        if(countAll==nrDigits){
            res=true;
            res=htons(res);
            if(send(sock,&res, sizeof(res),0)<0){
                perror("send");
            }

            if(recv(sock,&buff, sizeof(buff),0)<0){
                perror("recv");
            }
            printf("%s",buff);
            exit(0);
        }
        else{
        //digits between 0-9
        uint16_t randomDigit=rand()%10;
        randomDigit=htons(randomDigit);
        if(send(sock,&res,sizeof(res),0)<0){
            perror("send");
        }

        if(send(sock,&randomDigit, sizeof(randomDigit),0)<0){
            perror("send");
        }

        if(send(sock,&allDigits, sizeof(allDigits),0)<0){
            perror("send");
        }

        if(select(sock+1,&read_fds,NULL,NULL,NULL)<0){
            perror("select");
            exit(1);
        }


        if(FD_ISSET(sock,&read_fds)){
            nbytes=recv(sock,&pos,sizeof(pos),0);
            if(nbytes<0){
                perror("read");
                exit(1);
            }

            else {
                pos = ntohs(pos);
                randomDigit=ntohs(randomDigit);
                if (pos < nrDigits){
                    printf("Digit %d exists in nr\n", randomDigit);
                    allDigits[pos] = randomDigit;
                    countAll++;
            }
                else {
                    printf("Digit %d does not exist in number\n", randomDigit);
                }
            }
        }



        sleep(1);}
    }
    return 0;
}