#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "lsnr.h"
#include "option_parser.h"
#include "server.h"
#include "client.h"
#include "linked_list.h"
#include "net_member.h"
#include "payload_handler.h"
#include "cmd.h"

int main(int argc, char *argv[]) {

	struct Options options; // EVENTUALLY THIS SHOULD CONTAIN NAME; IF NONE IS PROVIDED IT SHOULD BE PASSED TO CLIENT IN HANDSHAKE AS Anonymous#%d%d%d%d

	initOpts(&options);
	parseOpts(argc, argv, &options);
	if (options.v) {
		printOpts(&options);
	}

	if (options.l) {
		NetMember *server = startServer(options.port, "Servalicious");
		NetMember *client = listenForConnection(server);
		// ADD CLIENT TO linked_list
		// WHEN THAT'S WORKING TURN THIS INTO A LOOP TO ACCEPT MANY CLIENTS
		// WHEN THAT'S WORKING MULTITHREAD SERVER TO ACCEPT CLIENTS AND PROPOGATE MSG'S SIMULTANEOUSLY
		freeNM(server);
		freeNM(client);
	} else {
		makeConnection(options.port);
	}

	return 0;
}
