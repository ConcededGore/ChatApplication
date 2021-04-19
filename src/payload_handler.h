#ifndef PAYLOADHANDLER
#define PAYLOADHANDLER

typedef enum CMD {
	BADCMD, // SHOULD NEVER EVER BE USED INTENTIONALLY, MERELY FOR RETURNING AN ERROR (currently just for atoCMD)
	// USER CMD's
	SENDMESG, // Used for sending plain text
	// JOINT CMD's
	DSCNCLNT, // Signals that a client is disconnecting
	MUTECLNT, // Mute a client
	UNMTCLNT, // Unmute a client
	// SYS CMD's
	UPDTUSRS, // Tells client to update its list of users
	HSHKINIT, // Begins handshake protocol
	HSHKRECV // Tells server handshake is recieved and furthers protocol
} CMD;

typedef struct CMDData {
	CMD cmd;
	int bodySize;
	int argc;
	char **argv;
} CMDData;

typedef struct Payload {
	char *header;
	char *body;
	int bodySize;
} Payload;

char* CMDtoa(CMD cmd);
CMD atoCMD(char *str);

Payload* genHSHKINIT(const char *srvName);

char* genCMDHeader(CMDData *data);
char* genCMDBody(CMDData *data);
char* getTimestamp();

CMDData* genCMDData(CMD cmd, int argc, char **argv);
CMDData* digestHeader(char *str); // Returns CMDData with bodySize and cmd set
CMDData* digestBody(char *str, CMDData *header); // Returns CMDData, with all set

int getCMDBodySize(CMDData *data);
int getNumArgs(CMD cmd);

CMDData* genCMDData(CMD cmd, int argc, char **argv);
void freeCMDData(CMDData *data);
void freePayload(Payload *payload);

#endif
