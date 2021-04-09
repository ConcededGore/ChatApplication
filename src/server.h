#ifndef SERVER
#define server

struct Client {
	int socket;
	char *name;
};

void listenForConnection(int port);
char* initHandshake(const char *name, int clientSocket);
char* getTimestamp();

#endif
