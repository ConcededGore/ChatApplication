#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "client.h"

void makeConnection(int port) {

	// Create server socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	// define server address
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	int conStatus = connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
	if (conStatus == -1) {
		printf("Connection Failed");
		return;
	}
	
	char *serverName = recvHandshake("Client", clientSocket);
	printf("Connected to: %s", serverName);

	// close socket
	close(clientSocket);

	// Free pointers
	free(serverName);
}

char* recvHandshake(const char *clientName, int cltSock) {
	char header[20];

	recv(cltSock, header, 20, 0);
	printf("GOT HEADER:\n%s\n", header);
	int bodySize = 0;
	strtok(header, " ");
	// THIS IS DANGEROUS WHEN MULTITHREADING!!!!!
	bodySize = atoi(strtok(NULL, "\0"));
	printf("BODY SIZE: %d\n", bodySize);

	char serverName[bodySize];
	recv(cltSock, serverName, bodySize, 0);

	char *retval = malloc(strlen(serverName) * sizeof(char));
	strcpy(retval, serverName);

	
	return retval;
}
