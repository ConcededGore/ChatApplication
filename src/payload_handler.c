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

char* genMSGHeader(char *MSG) {
	if (strlen(MSG) > 2147483647) { // \0 is not considered in strlen
		printf("ERROR: MSG LENGTH EXCEEDS MAXIMUM 32 BIT INTEGER VALUE!");
		return NULL;
	}

	char *retval = malloc(24 * sizeof(char)); // "CMD CMDIDENT 2147483647\0" is the longest possible header, 24 chars
	strcpy(retval, "MSG ");
	char intStr[10];
	sprintf(intStr, "%d", (int)strlen(MSG));
	strcat(retval, intStr);

	return retval;
}

// UNIMPLEMENTED
int getCMDBodySize() {
	return 0;
}

char* genCMDHeader(CMD cmd) {

	char *retval = malloc(24 * sizeof(char)); // DITTO ABOVE
	strcpy(retval, "CMD ");
	strcat(retval, CMDtoa(cmd));
	strcat(retval, " ");
	char intStr[10];
	sprintf(intStr, "%d", getCMDBodySize());
	strcat(retval, intStr);

	return retval;
}
