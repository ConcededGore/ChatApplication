#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "payload_handler.h"
#include "cmd.h"

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

	char **argv = malloc(2 * sizeof(char*));
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

	free(argv[0]);
	free(argv);
	return retval;
}

Payload* genHSHKRECV(const char *clntName) {
	Payload *retval = genHSHKINIT(clntName);
	retval->header[4] = 'R';
	retval->header[5] = 'E';
	retval->header[6] = 'C';
	retval->header[7] = 'V';
	return retval;
}

Payload* genMUTECLNT(int userid, int targetid) {
	Payload *retval = malloc(sizeof(Payload));
	CMDData data;
	int argc = 2;
	char **argv = malloc(argc * sizeof(char*));

	data.argc = argc;
	char buffer[11];
	buffer[10] = '\0';
	int i;
	for (i = 0; i < 10; i++) {
		buffer[i] = '0';
	}

	sprintf(buffer, "%d", userid);
	argv[0] = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(argv[0], buffer);

	sprintf(buffer, "%d", targetid);
	argv[1] = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(argv[1], buffer);

	data.argv = argv;
	data.bodySize = getCMDBodySize(&data);
	data.cmd = MUTECLNT;

	retval->header = genCMDHeader(&data);
	retval->body = genCMDBody(&data);
	retval->bodySize = data.bodySize;

	free(argv[0]);
	free(argv[1]);
	free(argv);
	return retval;
}

Payload* genUNMTCLNT(int userid, int targetid) {
	Payload *retval = genMUTECLNT(userid, targetid);
	retval->header[0] = 'U';
	retval->header[1] = 'N';
	retval->header[2] = 'M';
	retval->header[3] = 'T';

	return retval;
}

Payload* genDSCNCLNT(int userid) {
	Payload *retval = malloc(sizeof(Payload));
	CMDData data;
	int argc = 1;
	char **argv = malloc(argc * sizeof(char*));

	data.argc = argc;

	char buffer[11];
	buffer[10] = '\0';
	int i;
	for(i = 0; i < 10; i++) {
		buffer[i] = '0';
	}

	sprintf(buffer, "%d", userid);
	argv[0] = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(argv[0], buffer);

	data.cmd = DSCNCLNT;
	data.argv = argv;
	data.bodySize = getCMDBodySize(&data);

	retval->header = genCMDHeader(&data);
	retval->body = genCMDBody(&data);
	retval->bodySize = data.bodySize;

	free(argv[0]);
	free(argv);

	return retval;
}

Payload* genSENDMESG(int userid, char *MSG) {
	Payload *retval = malloc(sizeof(Payload));
	CMDData data;
	int argc = 3;
	char **argv = malloc(argc * sizeof(char*));

	data.argc = argc;
	argv[0] = getTimestamp();

	char buffer[11];
	buffer[10] = '\0';
	int i;
	for (i = 0; i < 10; i++) {
		buffer[i] = '0';
	}

	sprintf(buffer, "%d", userid);
	argv[1] = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(argv[1], buffer);

	argv[2] = malloc((strlen(MSG) + 1) * sizeof(char));
	strcpy(argv[2], MSG);

	data.cmd = SENDMESG;
	data.argv = argv;
	data.bodySize = getCMDBodySize(&data);

	retval->header = genCMDHeader(&data);
	retval->body = genCMDBody(&data);
	retval->bodySize = data.bodySize;

	free(argv[0]);
	free(argv[1]);
	free(argv[2]);
	free(argv);

	return retval;
}

char* genCMDHeader(CMDData *data) {

	char *retval = malloc(20 * sizeof(char)); // "CMDIDENT 2147483647\0" is the longest possible header, 20 chars
	strcpy(retval, CMDtoa(data->cmd)); // CMDIDENT
	strcat(retval, " ");
	char intStr[10];
	sprintf(intStr, "%d", data->bodySize); // BODYSIZE (int)
	strcat(retval, intStr);

	return retval;
}

CMDData* digestHeader(char *str) {
	char *ptr = str;
	char *numStart;

	char cmdStr[9];
	cmdStr[8] = '\0';

	char curr = ptr[0];

	int count = 0;
	int digFlag = 0;

	CMD cmd;

	CMDData *retval = malloc(sizeof(CMDData));

	while (curr != '\0') {
		if (count == 8) {
			cmd = atoCMD(cmdStr);
			count = 0;
			break;
		}
		cmdStr[count] = curr;
		ptr++;
		count++;
		curr = ptr[0];
	}

	if (cmd == BADCMD) {
		printf("digestHeader got a BADCMD cmd\n");
		freeCMDData(retval);
		return NULL;
	}

	retval->cmd = cmd;

	if (curr != ' ') {
		printf("digesterHeader did not find a space where it was expected\n");
		freeCMDData(retval);
		return NULL;
	}
	ptr++;

	numStart = ptr;
	curr = ptr[0];
	while(curr != '\0') {
		if (curr < 48 || curr > 57) {
			digFlag = 1;
			break;
		}
		ptr++;
		curr = ptr[0];
	}

	if (digFlag) {
		printf("digestHeader found latter half of header to be non-digits\n");
		freeCMDData(retval);
		return NULL;
	}

	retval->bodySize = atoi(numStart);

	retval->argc = getNumArgs(retval->cmd);
	retval->argv = malloc(retval->argc * sizeof(char *));

	return retval;
}

CMDData* digestBody(char *str, CMDData *header) {
	char *argStart = str;
	char *ptr = str;

	char curr = argStart[0];

	int argCount = 0;
	int msgFlag = (header->cmd == SENDMESG);
	while(curr != '\0') {
		if (curr == '\n') { // Should probably write a func to validate that number of \n's is as expected by argc

			// This feels very hacky but it should allow '\n' chars in MSG's if I desire them to be there
			if (msgFlag && argCount == 2) {
				header->argv[argCount] = malloc((strlen(argStart) + 1) * sizeof(char));
				strcpy(header->argv[argCount], argStart);
				break;
			}

			ptr[0] = '\0';
			ptr++;
			curr = ptr[0];
			if (curr == '\0') { // Since there is always a trailing '\n'
				break;
			}

			header->argv[argCount] = malloc((strlen(argStart) + 1) * sizeof(char));
			strcpy(header->argv[argCount], argStart);

			argCount++;
			argStart = ptr;

			continue;
		}
		ptr++;
		curr = ptr[0];
	}

	return header;

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

// HELPER FUNCTIONS

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

	if (retval[strlen(retval) - 1] == '\n') {
		retval[strlen(retval) - 1] = '\0';
	}

	return retval;
}
