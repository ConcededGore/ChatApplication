#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "server.h"

struct NetMember* startServer(int port, char *name) {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in *serverAddress = malloc(sizeof(struct sockaddr_in));
	struct NetMember *retval = malloc(sizeof(struct NetMember));

	serverAddress->sin_family = AF_INET;
	serverAddress->sin_port = htons(port);
	serverAddress->sin_addr.s_addr = INADDR_ANY; // localhost (I think)

	bind(serverSocket, (struct sockaddr*) serverAddress, sizeof(*serverAddress));

	retval->socket = serverSocket;
	retval->name = name;
	retval->addr = serverAddress;

	return retval;
}

struct NetMember* listenForConnection(struct NetMember *server) {
	struct NetMember *retval = malloc(sizeof(struct NetMember));
	//struct sockaddr_in *clientAddr = malloc(sizeof(struct sockaddr_in));

	// listen for connections
	listen(server->socket, 1);

	// first NULL would be struct address of where we want to store info about the accepted client, and then the next null is the size of the struct
	int clientSocket = accept(server->socket, NULL /*clientAddr*/, NULL /*sizeof(*clientAddr)*/);

	// Initiate handshake protocol
	char *clientName = initHandshake(server->name, clientSocket);

	retval->socket = clientSocket;
	retval->name = clientName;
	retval->addr = NULL; //clientAddr;

	// EVERYTHING BELOW SHOULDN'T BE IN THIS FUNCTION
	char *timestamp = getTimestamp();
	printf("Recieved connection from: %s at %s", clientName, timestamp);
	// close socket
	close(server->socket);
	// Free pointers
	free(clientName);
	free(timestamp);

	return retval;
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


