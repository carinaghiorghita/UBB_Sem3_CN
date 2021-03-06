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

struct sockaddr_in getSocketName(int s,bool local_or_remote){
    struct sockaddr_in addr;
    int addrlen=sizeof(addr);

    memset(&addr,0,addrlen);
    int ret=(local_or_remote==true?getsockname(s,(struct sockaddr*)&addr,(socklen_t*)&addrlen):
            getpeername(s,(struct sockaddr*)&addr,(socklen_t*)&addrlen));

    if(ret<0) perror("getSocketName");
    return addr;
}

char* getIPAddress(int s,bool local_or_remote){
    struct sockaddr_in addr;
    addr=getSocketName(s,local_or_remote);
    return inet_ntoa(addr.sin_addr);
}

int getPort(int s,bool local_or_remote){
    struct sockaddr_in addr;
    addr=getSocketName(s,local_or_remote);
    return addr.sin_port;
}

void sendToAll(char* buf,int nbytes){
    for(int j=0;j<=fdmax;++j)
        if(FD_ISSET(j,&master))
            if(j!=listener&&j!=crt)
                if(send(j,buf,nbytes,0)<0) perror("send");
}

int main(int argc, char **argv) {
    if(argc<2){
        printf("Usage:<port>\n");
        exit(1);
    }

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
    server.sin_port=htons(atoi(argv[1]));

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
                        client_count++;

                        if(newfd>fdmax){fdmax=newfd;} //keep max fd

                        printf("New connection from %s on socket %d\n",getIPAddress(newfd,false),newfd);

                        sprintf(buff,"Hi, you are client :[%d] (%s:%d) connected to server %s.\nThere are %d clients connected.\n",
                                newfd,getIPAddress(newfd,false),getPort(newfd,false),getIPAddress(listener,true),client_count);
                        send(newfd,buff,strlen(buff)+1,0);
                    }
                }
                else{ //new data from client
                    nbytes=recv(i,buff,sizeof(buff),0);
                    if(nbytes<=0){
                        if(nbytes==0) //connection closed
                            printf("Client %d forcibly hung up\n",i);
                        else
                            perror("recv");
                        client_count--;
                        close(i);
                        FD_CLR(i,&master);
                    }
                    else{
                        buff[nbytes]='\0';
                        if(strncasecmp(buff,"quit\n",4)==0){ //connection close request
                            sprintf(buff,"Request granted [%d] - %s. Disconnecting...\n",i,getIPAddress(i,false));
                            send(i,buff,strlen(buff)+1,0);

                            nbytes=sprintf(tmpbuff,"<%s - %d> disconnected\n",getIPAddress(i,false),i);
                            sendToAll(tmpbuff,nbytes);

                            client_count--;
                            close(i);
                            FD_CLR(i,&master);
                        }
                        else{ //we got actual data from a client
                            nbytes=sprintf(tmpbuff,"<%s - [%d]> %s",getIPAddress(i,false),i,buff);
                            sendToAll(tmpbuff,nbytes);
                        }
                    }
                }
            }
        }
    }
    return 0;
}
