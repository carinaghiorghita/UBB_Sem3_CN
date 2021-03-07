#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

struct sockaddr_in server,client;
fd_set master,read_fds;
int fdmax,client_count=0;
int listener,crt,newfd;
int yes=1,i,len,nbytes;
char buff[256];


int main(int argc, char **argv) {


    listener=socket(AF_INET,SOCK_STREAM,0);
    if(listener<0){
        perror("socket");
        exit(1);
    }

    if(setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))<0){
        perror("setsockopt");
        exit(1);
    }

    //bind
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(7000);

    if(bind(listener,(struct sockaddr*)&server,sizeof(server))<0){
        perror("bind");
        exit(1);
    }

    if(listen(listener,10)<0){
        perror("listen");
        exit(1);
    }

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    //add listener to master set
    FD_SET(listener,&master);

    //keep max fd
    fdmax=listener;

    uint16_t randomNumber=rand();
    uint16_t nrDigits=0,copyNr=randomNumber;
    uint16_t randomDigit;

    while(copyNr){
        nrDigits++;
        copyNr/=10;
    }

    nrDigits=htons(nrDigits);
    uint16_t digFromClient[nrDigits];

    while(1){
        read_fds=master;

        if(select(fdmax+1,&read_fds,NULL,NULL,NULL)<0){
            perror("select");
            exit(1);
        }

        for(i=0;i<=fdmax;++i){
            if(FD_ISSET(i,&read_fds)){  //we can read data from i
                crt=i;
                if(i==listener){ //new connection
                    len=sizeof(client);

                    newfd=accept(listener,(struct sockaddr*)&client,(socklen_t*)&len);
                    if(newfd<0){
                        perror("accept");
                        //exit(1);
                    }
                    else{
                        FD_SET(newfd,&master);
                        client_count++;

                        if(newfd>fdmax){fdmax=newfd;} //keep max fd

                        if(send(newfd,&nrDigits, sizeof(nrDigits),0)<0){
                            perror("send");
                            exit(1);
                        }
                    }
                }
                else{ //new data from client
                    bool res;
                    if(recv(i,&res,sizeof(res),0)<0){
                        perror("recv");
                    }
                    res=ntohs(res);
                    if(res==false) {
                        nbytes = recv(i, &randomDigit, sizeof(randomDigit), 0);
                        if (nbytes <= 0) {
                            perror("recv");
                        } else {
                            if (recv(i, &digFromClient, sizeof(digFromClient), 0) < 0) {
                                perror("recv");
                            }
                            randomDigit = ntohs(randomDigit);
                            uint16_t copy = randomNumber, k = 0;
                            while (copy) {
                                if (copy % 10 != randomDigit ||
                                    (copy % 10 == randomDigit && ntohs(digFromClient[k]) ==randomDigit)) {
                                    k++;
                                    copy /= 10;
                                }
                                else{break;}
                            }
                            k = htons(k);
                            if (send(i, &k, sizeof(k), 0) < 0) {
                                perror("send");
                            }
                        }
                    }
                    else{
                        sprintf(buff,"Winner is client %d with number %d",i,randomNumber);
                        for(int j=0;j<=fdmax;++j)
                            if(FD_ISSET(j,&master))
                                if(j!=listener){

                                if(send(j,&buff, sizeof(buff),0)<0) {
                                    perror("send");
                                }
                        }
                        exit(0);
                    }
                }
            }
        }
    }
    return 0;
}