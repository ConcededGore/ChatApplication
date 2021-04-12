#ifndef NETMEMBER
#define NETMEMBER

typedef struct NetMember {
	int id;
	int socket;
	char *name;
	struct sockaddr_in* addr;
} NetMember;

struct NetMember* createNetMember(int id, char *name);
void freeNM(struct NetMember* deall);

#endif
