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
#include "cmd.h"

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
	listen(server->socket, 1); // 1 means there can only be one connection enqueued

	// first NULL would be struct address of where we want to store info about the accepted client, and then the next null is the size of the struct
	int clientSocket = accept(server->socket, NULL /*clientAddr*/, NULL /*sizeof(*clientAddr)*/); // Need to figure out how to store addr info

	// Initiate handshake protocol
	CMDData *hshk = initHandshake(server->name, clientSocket);

	retval = createNetMember(1 /*SHOULDNT BE HARDCODED*/, hshk->argv[0]);
	retval->socket = clientSocket;
	retval->addr = NULL; //clientAddr;

	// EVERYTHING BELOW SHOULDN'T BE IN THIS FUNCTION
	printf("%s:\t%s connected!\n", hshk->argv[1], hshk->argv[0]);
	// close socket
	close(server->socket);
	// Free pointers
	freeCMDData(hshk);

	return retval;
}

CMDData* initHandshake(const char *name, int clntSock) {

	// Sending initial handshake
	Payload *handshakeSend = genHSHKINIT("Servalicious");
	send(clntSock, handshakeSend->header, 20, 0);
	send(clntSock, handshakeSend->body, handshakeSend->bodySize, 0);
	freePayload(handshakeSend);

	// Recieving handshake response
	char header[20];
	int bodySize = 0;
	CMDData *retval;

	recv(clntSock, header, 20, 0);
	retval = digestHeader(header);
	bodySize = retval->bodySize;
	char body[bodySize];
	recv(clntSock, body, bodySize, 0);
	digestBody(body, retval);

	return retval;
}
