#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "server.h"
#include "net_member.h"
#include "payload_handler.h"

NetMember* startServer(int port, char *name) {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in *serverAddress = malloc(sizeof(struct sockaddr_in));
	NetMember *retval = createNetMember(0, name);

	serverAddress->sin_family = AF_INET;
	serverAddress->sin_port = htons(port);
	serverAddress->sin_addr.s_addr = INADDR_ANY; // localhost (I think)

	bind(serverSocket, (struct sockaddr*) serverAddress, sizeof(*serverAddress));

	retval->socket = serverSocket;
	retval->addr = serverAddress;

	return retval;
}

NetMember* listenForConnection(struct NetMember *server) {
	NetMember *retval;
	//struct sockaddr_in *clientAddr = malloc(sizeof(struct sockaddr_in));

	// listen for connections
	listen(server->socket, 1);

	// first NULL would be struct address of where we want to store info about the accepted client, and then the next null is the size of the struct
	int clientSocket = accept(server->socket, NULL /*clientAddr*/, NULL /*sizeof(*clientAddr)*/); // Need to figure out how to store addr info

	// Initiate handshake protocol
	char *clientName = initHandshake(server->name, clientSocket);

	retval = createNetMember(1 /*SHOULDNT BE HARDCODED*/, clientName);
	retval->socket = clientSocket;
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
	// THIS NEEDS TO BE REWORKED AS IT IS HARDCODED
	char **args = malloc(sizeof(char*));
	args[0] = malloc(((int)strlen("Servalicious") + 1) * sizeof(char));
	strcpy(args[0], "Servalicious");
	CMDData *data = genCMDData(HSHKINIT, 1, args);

	char *header = genCMDHeader(data);
	send(cltSock, header, 20, 0);

	char *body = genCMDBody(data);
	// ADD bodySize TO CMDData
	send(cltSock, body, getCMDBodySize(data), 0);

	char *retval = malloc(256 * sizeof(char));
	strcpy(retval, "TEMP");

	free(data);
	return retval;
}

char* getTimestamp() {
	struct tm *tmPtr;
	time_t lt;
	char *retval;

	lt = time(NULL);
	tmPtr = localtime(&lt);
	retval = malloc(strlen(asctime(tmPtr)));
	strcpy(retval, asctime(tmPtr));

	return retval;
}
