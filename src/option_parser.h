#ifndef OPTION_PARSER
#define OPTION_PARSER

struct Options {
	int l; // Listen for connections from [ip]
	int p; // Specify port will be given as a range
	int v; // Verbose mode

	char* ip; // Ip to bind/connect to -------------------------------------FREE ME
	int port; // Port to listen on
	int pRange[2]; // Range of ports to listen on if -p is set

	int badopt; // Flag that is tripped when options cant be parsed
};

void parseOpts(int argc, char *argv[], struct Options *options);
void initOpts(struct Options *options);
void printOpts(struct Options *options);

#endif
