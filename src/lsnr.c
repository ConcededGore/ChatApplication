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
	
	struct Options options;

	initOpts(&options);
	parseOpts(argc, argv, &options);
	printOpts(&options);

	initConn(options);

	return 0;
}

void initConn(struct Options options) {
	if (options.l) {
		listenForConnection(options.port);

	} else {
		makeConnection(options.port);
	}
}
