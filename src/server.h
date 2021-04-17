#ifndef SERVER
#define SERVER

#include "net_member.h"

NetMember* startServer(int port, char *name);
NetMember* listenForConnection(NetMember *server);
char* initHandshake(const char *name, int clientSocket);

#endif
