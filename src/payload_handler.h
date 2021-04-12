#ifndef PAYLOADHANDLER
#define PAYLOADHANDLER

typedef enum CMD {
	// JOINT CMD's
	DSCNCLNT, // Disconnect client
	MUTECLNT, // Mute a client
	UNMTCLNT, // Unmute a client
	// SYS CMD's
	UPDTUSRS, // Tells client to update its list of users
	HSHKINIT, // Begins handshake protocol
	HSHKRECV // Tells server handshake is recieved and furthers protocol
} CMD;

char* CMDtoa(CMD cmd);
char* genMSGHeader(char *MSG);
char* genCMDHeader(CMD cmd);

#endif
