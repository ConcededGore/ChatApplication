#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "server.h"

void listenForConnection(int port) {

	// Create server socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	// define server address
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to our specified IP and Port
	bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

	// listen for connections
	listen(serverSocket, 1);

	// first NULL would be struct address of where we want to store info about the accepted client, and then the next null is the size of the struct
	int clientSocket = accept(serverSocket, NULL, NULL);

	// Initiate handshake protocol
	char *clientName = initHandshake("Server", clientSocket);
	char *timestamp = getTimestamp();
	printf("Recieved connection from: %s at %s", clientName, timestamp);

	// close socket
	close(serverSocket);

	// Free pointers
	free(clientName);
	//free(timestamp);
}

char* initHandshake(const char *name, int cltSock) {
	char *data = malloc(1024);

	strcpy(data, "HEAD ");
	strcat(data, name);
	strcat(data, " ");
	send(cltSock, data, strlen(data), 0);

	recv(cltSock, data, 1024, 0);
	char clientName[1024];
	strtok(data, " ");
	strcpy(clientName, strtok(NULL, " "));

	char *retval = malloc(strlen(clientName) * sizeof(char));
	strcpy(retval, clientName);

	free(data);
	return retval;
}

char* getTimestamp() {
	struct tm *tmPtr;
	time_t lt;
	char *retval;

	lt = time(NULL);
	tmPtr = localtime(&lt);
	retval = asctime(tmPtr);

	return retval;
}


