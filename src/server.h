#ifndef SERVER
#define SERVER

#include <sys/socket.h>

struct NetMember {
	int socket;
	char *name;
	struct sockaddr_in* addr;
};

struct NetMember* startServer(int port, char *name);
struct NetMember* listenForConnection(struct NetMember *server);
char* initHandshake(const char *name, int clientSocket);
char* getTimestamp();

#endif
