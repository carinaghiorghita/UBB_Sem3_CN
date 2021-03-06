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
char buff[256],tmpbuff[256];

struct connectedClients{
    uint16_t cPort;
    in_addr_t cIP;
} clients[255];

struct sockaddr_in getSocketName(int s,bool local_or_remote){
    struct sockaddr_in addr;
    int addrlen=sizeof(addr);

    memset(&addr,0,addrlen);
    int ret=(local_or_remote==true?getsockname(s,(struct sockaddr*)&addr,(socklen_t*)&addrlen):
             getpeername(s,(struct sockaddr*)&addr,(socklen_t*)&addrlen));

    if(ret<0) perror("getSocketName");
    return addr;
}

char* getIPAddress(int s, bool local_or_remote){
    struct sockaddr_in addr;
    addr=getSocketName(s,local_or_remote);
    return inet_ntoa(addr.sin_addr);
}

int getPort(int s,bool local_or_remote){
    struct sockaddr_in addr;
    addr=getSocketName(s,local_or_remote);
    return addr.sin_port;
}

void sendToAll(char* buf,int nrbytes){
    for(int j=0;j<=fdmax;++j)
        if(FD_ISSET(j,&master))
            if(j!=listener&&j!=crt)
                if(send(j,buf,nrbytes,0)<0) perror("send");
}


void addClientToList(int newfdd) {
    uint16_t newPort=getPort(newfdd,false);
    in_addr_t newIP=inet_addr(getIPAddress(newfdd,false));

    client_count++;
    clients[client_count].cIP=newIP;
    clients[client_count].cPort=newPort;
}

void sendClientListUpdateToAll(int excludedSocket, bool hasDisconnected){
    //don't send this update to newly connected client (the excludedSocket)
    for(int j=0;j<=fdmax;++j)
        if(FD_ISSET(j,&master))
            if(j!=listener&&j!=excludedSocket){
                //might be local_or_remote=true
                in_addr_t clientIP=htonl(inet_addr(getIPAddress(excludedSocket,true)));
                uint16_t clientPort=htons(getPort(excludedSocket,false));
                hasDisconnected=htons(hasDisconnected);

                if(send(j,&clientIP,sizeof(clientIP),0)<0){
                    perror("send IP");
                }

                if(send(j,&clientPort,sizeof(clientPort),0)<0){
                    perror("send port");
                }

                if(send(j,&hasDisconnected,sizeof(hasDisconnected),0)<0){
                    perror("send bool");
                }
            }
}

void sendClientList(int destSocket){
    int clListLen=htons(client_count);

    if(send(destSocket,&clListLen,sizeof(clListLen),0)<0){
        perror("send len");
    }

    for(int j=1;j<=client_count;++j){
        in_addr_t newIP=htonl(clients[j].cIP);
        if(send(destSocket,&newIP,sizeof(newIP),0)<0){
            perror("send IP");
        }

        uint16_t newPort=htons(clients[j].cPort);
        if(send(destSocket,&newPort,sizeof(newPort),0)<0){
            perror("send port");
        }
    }
}

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
                        if(newfd>fdmax){fdmax=newfd;} //keep max fd

                        addClientToList(newfd);
                        printf("New connection from %s : %d\n",getIPAddress(newfd,false),getPort(newfd,false));
                        sendClientListUpdateToAll(newfd,false);
                        sendClientList(newfd);
                    }
                }
                else{ //new data from client
                    nbytes=recv(i,buff,sizeof(buff),0);
                    if(nbytes<=0){
                        if(nbytes==0) //connection closed
                            printf("Client %d forcibly hung up\n",i);
                        else
                            perror("recv");
                        sendClientListUpdateToAll(i,true);
                        //client_count--;
                        close(i);
                        FD_CLR(i,&master);
                    }
                    else{
                        buff[nbytes]='\0';
                        if(strncasecmp(buff,"quit",4)==0){ //connection close request
                            printf("Request granted [%d] - %s. Disconnecting...\n",i,getIPAddress(i,false));

                            sendClientListUpdateToAll(i,true);
                            close(i);
                            FD_CLR(i,&master);
                        }

                    }
                }
            }
        }
    }
    return 0;
}
