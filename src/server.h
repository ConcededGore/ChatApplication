#ifndef SERVER
#define SERVER

#include "net_member.h"
#include "cmd.h"

NetMember* startServer(int port, char *name);
NetMember* listenForConnection(NetMember *server);
CMDData* initHandshake(const char *name, int clientSocket);

#endif
