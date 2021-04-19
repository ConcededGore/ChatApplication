#ifndef CLIENT
#define CLIENT

#include "payload_handler.h"

void makeConnection(int port);
CMDData* recvHandshake(const char *clientName, int cltSock);

#endif
