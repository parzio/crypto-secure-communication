#include "common.h"

int Handshake();

int main(int argc, char ** argv)
{
	int inputChannel, outputChannel;
	FILE *file;

	/* Mandatory arguments */
	if( !argv[1] || !argv[2] || !argv[3] || !argv[4] ) {
		fprintf(stderr,"client [server->client fifo] [client->server fifo] [password file] [username]\n");
		exit(1);
	}

	/* Create connection with the server */
	fprintf(stderr,"Create connection...\n");
	outputChannel = openChannel(argv[1]);
	inputChannel  = openChannel(argv[2]);
/*
	close_channel(cs_fifo_fd);
	close_channel(sc_fifo_fd);
*/
}
