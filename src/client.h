#ifndef CLIENT
#define CLIENT

void makeConnection(int port);
char* recvHandshake(const char *clientName, int cltSock);

#endif
