#include "option_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printOpts(struct Options *options) {
	if (options->badopt) {
		printf("BAD OPT TRIPPED\n");
		return;
	}
	printf("flags: {l: %d,p: %d,v: %d}\n", options->l, options->p, options->v);
	printf("ip/port: %s:%d", options->ip, options->port);
	if (options->p) {
		printf("-%d", options->pRange[1]);
	}
	printf("\n");
}

void initOpts(struct Options *options) {
	options->l = 0;
	options->p = 0;
	options->v = 0;

	options->ip = "127.0.0.1";

	options->port = 4444;
	options->pRange[0] = 4444;
	options->pRange[1] = 4444;
	options->badopt = 0;
}

void parseOpts(int argc, char *argv[], struct Options *options) { // CONVERT THIS TO A TOP DOWN PARSER FOR BETTER INPUT STRUCTURE
	int i;
	int j;
	char curr;
	int portFlag = 0;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			curr = argv[i][1];
			j = 1;
			while (curr != '\0') {

				if (curr == 'v') {
					options->v = 1;
				} else if (curr == 'l') {
					options->l = 1;
				} else if (curr == 'p') {
					options->p = 1;
				} else {
					options->badopt = 1;
					return;
				}

				j++;
				curr = argv[i][j];
			}
		}
		if (argv[i][0] > 47 && argv[i][0] < 58) {
			char *needle = ".\0";
			if (strstr(argv[i], needle) != NULL) { // Has . and should be an ip

				char *delim = ".\0";
				char *token = strtok(argv[i], delim);
				int count = 0;
				while (token != NULL && count < 4) {
					curr = token[0];
					j = 0;
					while (curr != '\0') {
						if (!isdigit(curr)) {
							options->badopt = 1;
							return;
						}
						j++;
						curr = token[j];
					}

					if (atoi(token) > 255) {
						printf("IP OUT OF RANGE\n");
						options->badopt = 1;
						return;
					}
					token = strtok(NULL, delim);
					count++;
				}
				if (token != NULL && count >= 4) {
					options->badopt = 1;
					return;
				}

				char *ipStr = malloc(16 * sizeof(char));
				options->ip = strcpy(ipStr, argv[i]);

			} else { // No period and therefore should be a port #

				curr = argv[i][0];
				j = 0;
				int tacFlag = 0;
				while (curr != '\0') {
					if (!isdigit(curr) || curr == '-') {
						if (curr == '-' && !tacFlag) {
							if (!options->p) {
								printf("Please use option -p in order to specify a port range\n");
								options->badopt = 1;
								return;
							} else {
								tacFlag = 1;
							}
						} else {
							options->badopt = 1;
							return;
						}
					}
					j++;
					curr = argv[i][j];
				}

				options->port = atoi(argv[i]);
				portFlag = 1;
				if (options->p) {
					char *arg = argv[i];
					options->pRange[0] = options->port;
					strtok(arg, "-");
					options->pRange[0] = options->port;
					options->pRange[1] = atoi(strtok(NULL, "\0"));
					if (options->pRange[1] < options->pRange[0]) {
						printf("Port range should be in ascending order.\n");
						options->badopt = 1;
						return;
					}
				}
			}
		}
	}
	if (!portFlag && options->v) {
		printf("No port provided, defaulting to %d\n", options->port);
	}
}
