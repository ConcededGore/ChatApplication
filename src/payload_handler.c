#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "payload_handler.h"

char* CMDtoa(CMD cmd) {
	char *retval = malloc(9 * sizeof(char));
	switch(cmd) {
		// JOINT CMD's
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
			printf("ERROR: CMDtoa IS DEFAULTING FROM cmd = %d", cmd);
			return NULL;
	}
}

// UNIMPLEMENTED
int getCMDBodySize(CMD cmd) {
	return 0;
}

char* genCMDHeader(CMD cmd) {

	char *retval = malloc(20 * sizeof(char)); // "CMDIDENT 2147483647\0" is the longest possible header, 20 chars
	strcpy(retval, "CMD ");
	strcat(retval, CMDtoa(cmd));
	strcat(retval, " ");
	char intStr[10];
	sprintf(intStr, "%d", getCMDBodySize(cmd));
	strcat(retval, intStr);

	return retval;
}

// BE CAREFUL THAT FOR MSG's THE ACTUAL TEXT IS THE VERY LAST ARG SO IT MAY CONTAIN \n WITHOUT ISSUE (or escape chars cuz I'm lazy)
char* genCMDBody(CMDData data) {
	char *retval = malloc(25 * sizeof(char));
	strcpy(retval, "UNIMPLEMENTED\n");
	return retval;
}

// ADD FUNC'S TO AUTOGEN ARGC AND ARGV (this would also allow the data to be freed automatically!!!)
CMDData* genCMDData(CMD cmd, int argc, char **argv) {

	CMDData *retval = malloc(sizeof(CMDData));

	retval->cmd = cmd;
	retval->argc = argc;
	retval->argv = argv;

	return retval;
}

void freeCMDData(CMDData *data) {
	int i;
	for (i = 0; i < data->argc; i++) {
		free(data->argv[i]);
	}
	free(data);
}
