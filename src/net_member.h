#ifndef NETMEMBER
#define NETMEMBER

typedef struct NetMember {
	int id;
	int socket;
	char *name;
	struct sockaddr_in* addr;
} NetMember;

NetMember* createNetMember(int id, char *name);
void freeNM(NetMember* deall);

#endif
