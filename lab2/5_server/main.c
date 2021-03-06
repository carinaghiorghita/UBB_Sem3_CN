#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>

int c;

void zombieHandler(int sgn){
    wait(0);
}

void timeOut(int sgn){
    uint16_t r=-1;
    r=htons(r);
    printf("Process timed out.");

    send(c,&r,sizeof(r),0);
    close(c);
    exit(1);
}

void treatFork(){
    if(c<0){
        perror("accept");
        exit(1);
    }

    signal(SIGALRM,timeOut);
    alarm(20);

    uint16_t n,arr[100],k=1;
    recv(c,&n,sizeof(n),0);
    n=ntohs(n);

    arr[0]=1;
    arr[0]=htons(arr[0]);
    if(n!=1) {
        arr[1] = n;
        arr[1] = htons(arr[1]);
        k++;
    }
    for(int i=2;i*i<=n;++i){
        if(n%i==0){
            arr[k]=i;
            arr[k]=htons(arr[k]);
            k++;
            if(i!=n/i) {
                arr[k] = n / i;
                arr[k] = htons(arr[k]);
                k++;
            }
        }
    }


    int K=k;
    k=htons(k);

    alarm(0);

    send(c,&k,sizeof(k),0);
    send(c,&arr,K*sizeof(uint16_t),0);

    close(c);

    exit(0);
}

int main() {
    struct sockaddr_in server,client;

    int s=socket(AF_INET, SOCK_STREAM,0);
    if(s<0){
        perror("socket");
        exit(1);
    }

    server.sin_port=htons(1500);
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_family=AF_INET;

    if(bind(s,(struct sockaddr*)&server,sizeof(server))<0){
        perror("bind");
        exit(1);
    }

    signal(SIGCHLD,zombieHandler);
    listen(s,5);

    while(1){
        int len=sizeof(client);
        memset(&client,0,len);

        c=accept(s,(struct sockaddr*)&client,&len);
        if(c<0){
            perror("accept");
            exit(1);
        }
        printf("Receiving data from client %s on TCP port %hu\n",inet_ntoa(client.sin_addr),client.sin_port);

        if(fork()==0){
            treatFork();
        }
    }
    return 0;
}
