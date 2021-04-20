#ifndef PAYLOADHANDLER
#define PAYLOADHANDLER

#include "cmd.h"

typedef struct Payload {
	char *header;
	char *body;
	int bodySize;
} Payload;

Payload* genHSHKINIT(const char *srvName);
Payload* genHSHKRECV(const char *clntName);
Payload* genDSCNCLNT(int userid);
Payload* genMUTECLNT(int userid, int targetid);
Payload* genUNMTCLNT(int userid, int targetid);
Payload* genSENDMESG(int userid, char *MSG);

char* genCMDHeader(CMDData *data);
char* genCMDBody(CMDData *data);
char* getTimestamp();

// str is the raw header/body recieved from the socket || SHOULD RENAME THIS!!!
CMDData* digestHeader(char *str); // Returns CMDData with bodySize and cmd set
CMDData* digestBody(char *str, CMDData *header); // Returns CMDData, with all set

int getCMDBodySize(CMDData *data);

void freePayload(Payload *payload);

#endif
