#ifndef PAYLOADHANDLER
#define PAYLOADHANDLER

typedef enum CMD {
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
	int argc;
	char **argv;
} CMDData;

char* CMDtoa(CMD cmd);

char* genCMDHeader(CMDData *data);
char* genCMDBody(CMDData *data);

int getCMDBodySize(CMDData *data);

CMDData* genCMDData(CMD cmd, int argc, char **argv);
void freeCMDData(CMDData *data);

#endif
