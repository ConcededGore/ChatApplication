#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int getCMDBodySize(CMDData *data) {
	int retval = 0;
	int i;

	for (i = 0; i < data->argc; i++) {
		retval += (int)strlen(data->argv[i]);
		retval += 1; // Adding a \n after every arg, will then append a '\0' in digest func
	}

	return retval;
}

Payload* genHSHKINIT(const char *srvName) {
	Payload *retval = malloc(sizeof(Payload));
	CMDData data;

	char **argv = malloc(sizeof(char*));
	argv[0] = malloc((strlen(srvName) + 1) * sizeof(char));
	argv[0][0] = '\0';
	strcpy(argv[0], srvName);
	char *timestamp = getTimestamp();
	argv[1] = malloc((strlen(timestamp) + 1) * sizeof(char));
	argv[1][0] = '\0';
	strcpy(argv[1], timestamp);

	data.argc = 2;
	data.argv = argv;
	data.cmd = HSHKINIT;
	data.bodySize = getCMDBodySize(&data); // GET RID OF BODYSIZE IN CMDData

	retval->header = genCMDHeader(&data);
	retval->body = genCMDBody(&data);
	retval->bodySize = data.bodySize;

	return retval;
}

char* genCMDHeader(CMDData *data) {

	char *retval = malloc(20 * sizeof(char)); // "CMDIDENT 2147483647\0" is the longest possible header, 20 chars
	strcat(retval, CMDtoa(data->cmd)); // CMDIDENT
	strcat(retval, " ");
	char intStr[10];
	sprintf(intStr, "%d", data->bodySize); // BODYSIZE (int)
	strcat(retval, intStr);

	return retval;
}

// BE CAREFUL THAT FOR MSG's THE ACTUAL TEXT IS THE VERY LAST ARG SO IT MAY CONTAIN \n WITHOUT ISSUE (or escape chars cuz I'm lazy)
char* genCMDBody(CMDData *data) {
	int bodySize = getCMDBodySize(data);
	if (bodySize < 1) {
		printf("ERROR: bodySize in genCMDBody < 1!\n");
		return NULL;
	}

	char *retval = malloc(bodySize * sizeof(char));
	retval[0] = '\0';
	int i;
	
	for (i = 0; i < data->argc; i++) {
		strcat(retval, data->argv[i]);
		strcat(retval, "\n");
	}

	if (strlen(retval) != bodySize) {
		printf("ERROR: genCMDBody has generated a retval of size %d, but expected to generate one of size %d\n", (int)strlen(retval), bodySize);
		printf("Body: %s\n", retval);
		return NULL;
	}

	return retval;
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

void freePayload(Payload *payload) {
	free(payload->header);
	free(payload->body);
	free(payload);
}

char* getTimestamp() {
	struct tm *tmPtr;
	time_t lt;
	char *retval;

	lt = time(NULL);
	tmPtr = localtime(&lt);
	retval = malloc(strlen(asctime(tmPtr)));
	strcpy(retval, asctime(tmPtr));

	return retval;
}
