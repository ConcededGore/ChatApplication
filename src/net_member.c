#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "net_member.h"

NetMember* createNetMember(int id, char *name) {
	NetMember *retval = malloc(sizeof(NetMember));
	retval->id = id;
	retval->name = malloc(strlen(name));
	strcpy(retval->name, name);
	return retval;
}

void freeNM(NetMember *deall) {
	free(deall->name);
	free(deall->addr);
	free(deall);
}
