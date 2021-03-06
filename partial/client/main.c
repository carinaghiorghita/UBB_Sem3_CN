#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

fd_set master,read_fds;
int sock,udpsock;
struct sockaddr_in server,client;
char buff[256];
int nbytes,fdmax,TCPPort,client_count;

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

void readClientList(){
    if(read(sock,&client_count, sizeof(client_count))<=0){
        printf("Server has closed connection\n");
        exit(2);
    }

    client_count=ntohs(client_count);
    printf("List size = %d\n", client_count);
    for(int i=1;i<=client_count;++i){
        in_addr_t IP;uint16_t port;

        if(read(sock,&IP, sizeof(IP))<0){
            perror("recv IP");
        }

        if(read(sock,&port, sizeof(port))<0){
            perror("recv port");
        }

        clients[i].cIP=ntohl(IP);
        clients[i].cPort=ntohs(port);
    }
}

void sendToAllClients(char* buf,int nrbytes){
    for(int j=1;j<=client_count;++j){
//        char *addr = NULL;
//        inet_ntop(AF_INET,&clients[j].cIP,addr, sizeof(addr));
//        if(clients[j].cPort!=TCPPort && strcmp(addr, getIPAddress(sock, 1))!=0){
        if(clients[j].cPort!=TCPPort){
            struct sockaddr_in newClient;

            newClient.sin_family=AF_INET;
            newClient.sin_addr.s_addr=clients[j].cIP;
            newClient.sin_port=htons(clients[j].cPort+100);

            if(sendto(udpsock,buf,nrbytes,0,(struct sockaddr*)&newClient, sizeof(newClient))<0){
                perror("sendto");
            }
        }
    }
}

void removeClient(in_addr_t clIP,uint16_t clPort){
    for(int j=1;j<=client_count;++j){
//        char* addr = NULL;
//        inet_ntop(AF_INET,&clients[j].cIP,addr, sizeof(addr));
//        if(clients[j].cPort==clPort&&strcmp(addr, getIPAddress(sock, 1))!=0){
        if(clients[j].cPort==clPort){
            for(int k=j;k<client_count;++k)
                clients[k]=clients[k+1];
            client_count--;
            return;
        }
    }
}
void printClient(in_addr_t address, uint16_t port){
    struct in_addr IPStruct; // this is just so I can print the address nicely
    IPStruct.s_addr = address;
    printf("%s : %hu\n", inet_ntoa(IPStruct), port);
}
void printClientList(){
    printf("Client list:\n");
    for (int i = 1; i <= client_count; i++){
        printClient(clients[i].cIP, clients[i].cPort);
    }
    printf("\n\n");
}
int main(int argc,char** argv) {

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        exit(1);
    }

    fdmax=sock;

    memset(&server,0, sizeof(server));
    server.sin_port=htons(7000);
    server.sin_addr.s_addr=inet_addr("192.168.75.128");
    server.sin_family=AF_INET;

    if(connect(sock,(struct sockaddr*)&server, sizeof(server))<0){
        perror("connect");
        exit(1);
    }

    readClientList();
    printClientList();

    TCPPort=getPort(sock,1);

    udpsock=socket(AF_INET,SOCK_DGRAM,0);
    if(udpsock<0){
        perror("udp socket");
        exit(1);
    }

    if(udpsock>fdmax) fdmax=udpsock;

    memset(&client,0, sizeof(client));
    client.sin_addr.s_addr=inet_addr(getIPAddress(sock,true));
    client.sin_port=htons(TCPPort+100);
    client.sin_family=AF_INET;

    if(bind(udpsock,(struct sockaddr*)&client, sizeof(client))<0){
        perror("bind");
        exit(1);
    }

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(0,&master);
    FD_SET(sock,&master);
    FD_SET(udpsock,&master);

    while(1){
        read_fds=master;

        if(select(fdmax+1,&read_fds,NULL,NULL,NULL)<0){
            perror("select");
            exit(1);
        }

        if(FD_ISSET(0,&read_fds)){ //read from stdin
            memset(&buff,0, sizeof(buff));
            nbytes=read(0,buff, sizeof(buff)-1);
            buff[nbytes]='\0';
            if(nbytes<0){
                perror("read from keyboard");
            }

            if(strncasecmp("quit",buff,4)==0){
                if(send(sock,buff, sizeof(buff),0)<0){
                    perror("send");
                }
                close(udpsock);
                close(sock);
                FD_CLR(sock,&master);
                FD_CLR(udpsock,&master);
                exit(0);
            }
            else{
                //undeva aici nu merge (printeaza chestii dupa quit dar nu trimite nimic)
                sendToAllClients(buff,nbytes);

            }
        }

        if(FD_ISSET(sock,&read_fds)){ //client list updates
            in_addr_t clIP;uint16_t clPort;
            bool hasDisconnected;
            int r=read(sock,&clIP,sizeof(clIP));
            if(r<0){
                perror("recv IP");
            }
            else if(r==0){
                printf("Server disconnected...");
                exit(0);
            }
            if(read(sock,&clPort,sizeof(clPort))<0){
                perror("recv port");
            }
            if(read(sock,&hasDisconnected,sizeof(hasDisconnected))<0){
                perror("recv bool");
            }

            clIP=ntohl(clIP);
            clPort=ntohs(clPort);
            hasDisconnected=ntohs(hasDisconnected);

            if(hasDisconnected==false){
                client_count++;
                clients[client_count].cIP=clIP;
                clients[client_count].cPort=clPort;
                printf("Connected ");
            }
            else{
                removeClient(clIP,clPort);
                printf("Disconnected ");
            }
            printClient(clIP, clPort);
            printClientList();
        }

        if(FD_ISSET(udpsock,&read_fds)){ //from other clients
            memset(&buff,0, sizeof(buff));
            int clLen= sizeof(client);
            if(recvfrom(udpsock,&buff, sizeof(buff),0,NULL,NULL)<=0){
                perror("recvfrom");
            }
            else printf("New message: %s\n",buff);
        }
    }
    return 0;
}