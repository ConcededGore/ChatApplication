#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "net_member.h"

struct NetMember* createNetMember(int id, char *name) {
	struct NetMember *retval = malloc(sizeof(struct NetMember));
	retval->id = id;
	retval->name = malloc(strlen(name));
	strcpy(retval->name, name);
	return retval;
}

void freeNM(NetMember *deall) {
	printf("TEST\n");
	free(deall->name);
	printf("TEST1\n");
	free(deall->addr);
	printf("TEST2\n");
	free(deall);
}
