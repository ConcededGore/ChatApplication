#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "client.h"
#include "payload_handler.h"

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

CMDData* recvHandshake(const char *clientName, int cltSock) {
	char header[20];

	recv(cltSock, header, 20, 0);

	int bodySize = 0;
		
	CMDData *retval = digestHeader(header);

	bodySize = retval->bodySize;

	char body[bodySize];
	recv(cltSock, body, bodySize, 0);

	digestBody(body, retval);
	
	return retval;
}
