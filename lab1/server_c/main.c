#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 1500
#define MAX_MSG 100

int main(int argc, char* argv[]) {
	struct sockaddr_in clientAddr, servAddr;

	int sd= socket(AF_INET, SOCK_STREAM, 0);

	if (sd < 0) {
		perror("Cannot open socket");
		return ERROR;
	}

	//bind server port
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERVER_PORT);

	if (bind(sd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("Cannot bind port");
		return ERROR;
	}

	listen(sd, 5);
	while (1) {
		printf("%s: Waiting for data on port TCP %u\n",argv[0],SERVER_PORT);

		int clientLen = sizeof(servAddr);
		int newSd = accept(sd,(struct sockaddr_in*)&clientAddr,&clientLen);

		if (newSd < 0) {
			perror("Cannot accept connection");
			return ERROR;
		}

		//initialise line
		char line[MAX_MSG];
		memset(line,0,MAX_MSG);

		//receive message
		int len;
		if (len = read(newSd, line, MAX_MSG) > 0) {
			printf("%s received from %s:TCP %s%d : %s\n", argv[0], inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), line);
			write(newSd, line, len);
		}
		else printf("Error receiving data");

		close(newSd);
	}

	return 0;
}
