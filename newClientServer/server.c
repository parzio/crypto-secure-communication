#include "common.h"

struct ciao{
	int x, y;

	void print() { printf("ciao"); }
} ;



int openFifo(const char * pathname){
	/* Recreate the FIFO in pathname */
	unlink(pathname);
	if(mkfifo(pathname,0600) < 0 ) {
		perror("mkfifo()");
		return -1;
	}

	/* Open the channel */
	return (openChannel(pathname));
}

int waitConnection(int inputChannel , u_int8_t **msg){
	
	fprintf(stderr,"Waiting for connection\n");

	if(readFromPipe(inputChannel , msg) < 0)
	{
		perror("connection error");
		return -1;
	}
	
	return 1;
}

int handshake(int inputChannel , int outputChannel, u_int8_t **first_msg){
	
	//TODO set keys , protocols ...
	
	if(strcmp((const char *)first_msg , (const char *) ClientOpenConnection) != 0)
	{
		perror("handshake error \n");
		return -1;
	}
	
	if(writeInPipe(outputChannel , (const u_int8_t **) ServerOpenConnection) < 0)
	{
		perror("handshake write error");
		return -1;
	}
	
	return 1;
	
}

int main(int argc, char ** argv)
{
	int inputChannel, outputChannel;

	/* Mandatory arguments */
	if( !argv[1] || !argv[2] || !argv[3] ) {
		fprintf(stderr,"server [server->client fifo] [client->server fifo] [password file]\n");
		exit(1);
	}
	
	/* Open the server->client fifo */
	fprintf(stderr,"Opening server->client fifo...\n");
	outputChannel = openFifo(argv[1]);

	/* Open the client->server fifo */
	fprintf(stderr,"Opening client->server fifo...\n");
	inputChannel = openFifo(argv[2]);
	
	do{
		u_int8_t **msg = NULL;
		int size;

		if(waitConnection(inputChannel, msg) < 0 ) {
			fprintf(stderr,"Communication error...\n");
			continue;
		}

		if(handshake(inputChannel , outputChannel , msg) < 0){
			fprintf(stderr,"Communication error...\n");
			continue;
		}
		
		do{
			
			size = readFromPipe(inputChannel , msg);
			if(size < 0)
			{
				perror("error while reading");
				break;				
			}
			
			fprintf(stderr,"Communication error...\n");
			
			//print msg
			
			u_int8_t **ssss = (u_int8_t **) "pene\0";
			
			if(writeInPipe(outputChannel , (const u_int8_t **) ssss) < 0)
			{
				perror("handshake write error");
				return -1;
			}
			
			//communication

		}while(1);
		
		//close communication
		

	}while(1);
	
	
	



	exit(0);
}

