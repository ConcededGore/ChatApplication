#ifndef SERVER
#define SERVER

#include "net_member.h"
#include "payload_handler.h"

NetMember* startServer(int port, char *name);
NetMember* listenForConnection(NetMember *server);
CMDData* initHandshake(const char *name, int clientSocket);

#endif
