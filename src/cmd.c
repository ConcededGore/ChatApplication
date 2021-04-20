#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "payload_handler.h"

char* CMDtoa(CMD cmd) {
	char *retval = malloc(9 * sizeof(char));
	retval[0] = '\0';
	switch(cmd) {
		// JOINT CMD's
		case SENDMESG:
			strcpy(retval, "SENDMESG");
			return retval;
		case DSCNCLNT:
			strcpy(retval, "DSCNCLNT");
			return retval;
		case MUTECLNT:
			strcpy(retval, "MUTECLNT");
			return retval;
		case UNMTCLNT:
			strcpy(retval, "UNMTCLNT");
			return retval;
		// SYS CMD's
		case UPDTUSRS:
			strcpy(retval, "UPDTUSRS");
			return retval;
		case HSHKINIT:
			strcpy(retval, "HSHKINIT");
			return retval;
		case HSHKRECV:
			strcpy(retval, "HSHKRECV");
			return retval;
		default:
			printf("ERROR: CMDtoa IS DEFAULTING FROM cmd = %d\n", cmd); // THIS IS FOR IF I ADD NEW CMD's AND FORGET TO UPDATE THIS FUNC
			return NULL;
	}
}

CMD atoCMD(char *str) {

	if (!strcmp(str, "DSCNCLNT")) {
		return DSCNCLNT;
	} else if (!strcmp(str, "MUTECLNT")) {
		return MUTECLNT;
	} else if (!strcmp(str, "UNMTCLNT")) {
		return UNMTCLNT;
	} else if (!strcmp(str, "UPDTUSRS")) {
		return UPDTUSRS;
	} else if (!strcmp(str, "HSHKINIT")) {
		return HSHKINIT;
	} else if (!strcmp(str, "HSHKRECV")) {
		return HSHKRECV;
	} else if (!strcmp(str, "SENDMESG")) {
		return SENDMESG;
	}

	printf("atoCMD is defaulting with str: %s\nReturning 0 (BADCMD)\n", str);
	return BADCMD;
}

int getNumArgs(CMD cmd) {
	switch(cmd) {
		case SENDMESG:
			return 3;
		case HSHKINIT:
		case HSHKRECV:
		case MUTECLNT:
		case UNMTCLNT:
			return 2;
		case DSCNCLNT:
		case UPDTUSRS:
			return 1;
		default:
			printf("getNumArgs is defaulting with CMD: %d\n", cmd);
			return 0;
	}
}

// ADD FUNC'S TO AUTOGEN ARGC AND ARGV (this would also allow the data to be freed automatically!!!)
CMDData* genCMDData(CMD cmd, int argc, char **argv) {

	CMDData *retval = malloc(sizeof(CMDData));

	retval->cmd = cmd;
	retval->argc = argc;
	retval->argv = argv;
	retval->bodySize = getCMDBodySize(retval);

	return retval;
}

void freeCMDData(CMDData *data) {
	int i;
	for (i = 0; i < data->argc; i++) {
		free(data->argv[i]);
	}
	free(data);
}
