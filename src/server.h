#ifndef SERVER
#define server

void listenForConnection(int port);
char* initHandshake(const char *name, int clientSocket);
char* getTimestamp();

struct Client() {
	int socket;
	char *name;
}

#endif
