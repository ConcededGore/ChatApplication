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

int main(int argc, char *argv[]) {
	
	struct Options options; // EVENTUALLY THIS SHOULD CONTAIN NAME; IF NONE IS PROVIDED IT SHOULD BE PASSED TO CLIENT IN HANDSHAKE AS Anonymous#%d%d%d%d

	initOpts(&options);
	parseOpts(argc, argv, &options);
	printOpts(&options);

	if (options.l) {
		struct NetMember *server = startServer(options.port, "Servalicious");
		struct NetMember *client = listenForConnection(server);
	} else {
		makeConnection(options.port);
	}

	// NOT FREEING server OR client!!!
	return 0;
}
