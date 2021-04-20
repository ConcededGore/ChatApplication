#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "client.h"
#include "payload_handler.h"
#include "cmd.h"

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

	CMDData *hshkData = recvHandshake("Client", clientSocket);
	printf("Connected to: %s at %s\n", hshkData->argv[0], hshkData->argv[1]);

	// close socket
	close(clientSocket);

	// Free pointers
	freeCMDData(hshkData);
}

CMDData* recvHandshake(const char *clientName, int clntSock) {
	// Getting initial handshake
	char header[20];
	int bodySize = 0;
	CMDData *retval;

	recv(clntSock, header, 20, 0);
	retval = digestHeader(header);
	bodySize = retval->bodySize;
	char body[bodySize];
	recv(clntSock, body, bodySize, 0);
	digestBody(body, retval); // This returns the pointer to body, but since its a pointer also alters it directly

	// Sending response to handshake
	Payload *hshkrecv = genHSHKRECV(clientName);
	send(clntSock, hshkrecv->header, 20, 0);
	send(clntSock, hshkrecv->body, hshkrecv->bodySize, 0);

	// Free pointers
	freePayload(hshkrecv);

	return retval;
}
