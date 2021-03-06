#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>

int main() {
	SOCKET s;
	struct sockaddr_in server;
    uint16_t a, b, sum;

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {

        printf("Error initializing the Windows Sockets Library");

        return -1;

    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("Error creating socket");
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(1234);
    server.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr*)&server, sizeof(server))<0) {
        printf("Error binding port");
        return -1;
    }

    listen(s, 5);
    struct sockaddr_in client;
    int len = sizeof(client);
    memset(&client, 0, sizeof(client));

    while (1) {

        int c = accept(s, (struct sockaddr*)&client, &len);
        int err = WSAGetLastError();

        if (c < 0) {
            printf("Accept error %d", err);
            return -1;
        }

        printf("Incoming connected client from: %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        int res = recv(c, (char*)&a, sizeof(a), 0);
        if (res != sizeof(a)) printf("Error receiving operand\n");

        res = recv(c, (char*)&b, sizeof(b), 0);
        if(res!=sizeof(b)) printf("Error receiving operand\n");

        a = ntohs(a);
        b = ntohs(b);
        sum = a + b;
        sum = htons(sum);

        res = send(c, (char*)&sum, sizeof(sum), 0);
        if(res!=sizeof(sum)) printf("Error sending sum\n");

        closesocket(c);
    }
    WSACleanup();
}