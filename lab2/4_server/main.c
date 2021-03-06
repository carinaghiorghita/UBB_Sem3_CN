#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>

int c;

void zombieHandler(int sgn){
    wait(0);
}

void timeOut(int sgn){
    uint16_t r=-1;
    r=htons(r);

    printf("Time out\n");
    send(c,r,sizeof(r),0);
    close(c);
    exit(1);
}

void treatFork(){
    char a[255],b[255],res[255];
    uint16_t n,m;

    if(c<0){
        perror("accept");
        exit(1);
    }

    signal(SIGALRM, timeOut);
    alarm(30);

    recv(c,a,255,0);
    recv(c,b,255,0);
    n=strlen(a)-1;
    m=strlen(b)-1;
    int i=0,j=0,k=0;

    while(i<n&&j<m){
        if(a[i]<=b[j]) res[k++]=a[i++];
        else res[k++]=b[j++];
    }
    while(i<n)res[k++]=a[i++];
    while(j<m)res[k++]=b[j++];

    res[k]='\0';
    alarm(0);
    send(c,res,255,0);

    close(c);
    exit(0);
}

int main() {
    struct sockaddr_in server,client;

    int sd=socket(AF_INET,SOCK_STREAM,0);
    if(sd<0){
        perror("socket");
        exit(1);
    }

    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_family=AF_INET;
    server.sin_port=htons(1500);

    if(bind(sd,(struct sockaddr*)&server,sizeof(server))<0){
        perror("bind");
        exit(1);
    }

    listen(sd,5);
    signal(SIGCHLD,zombieHandler);

    while(1){
        int len=sizeof(client);
        memset(&client,0,len);

        c=accept(sd,(struct sockaddr*)&client,&len);
        if(c>0) printf("Client %s connected on port %d",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

        if(fork()==0){
            treatFork();
        }
    }
    return 0;
}
