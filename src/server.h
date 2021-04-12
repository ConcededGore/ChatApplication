#ifndef SERVER
#define SERVER

#include "net_member.h"

struct NetMember* startServer(int port, char *name);
struct NetMember* listenForConnection(struct NetMember *server);
char* initHandshake(const char *name, int clientSocket);
char* getTimestamp();

#endif
