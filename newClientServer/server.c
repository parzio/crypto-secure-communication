#include "common.h"

typedef enum serverState {
	WAITING , 			//Waiting for connection + handshake
	HANDSHAKE , 		//Handshake completed, authenication
	AUTHENTICATED ,   //Authentication done , key Exchange phase
	COMMUNICATION , 	//Key and protocol established, communication
	CLOSING,				//Closing message received, closing the connection
	ERROR					//Error, forcing shutdown
} serverState;

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

int handshake(int inputChannel, int outputChannel , char * msg){

	if(strncmp(msg , ClientOpenConnection , strlen(ClientOpenConnection)) != 0)
	{
		fprintf(stderr, "Handshake error, the client hello is wrong\n");
		return -1;
	}
	if(writeInPipe(outputChannel,(byte *) ServerOpenConnection , strlen(ServerOpenConnection)) < 0)
	{
		fprintf(stderr, "Handshake error, the server couldn't send the hello message\n");
		return -1;
	}

	return 1;
}

int closeConnection(int inputChannel , int outputChannel){
	
	if(writeInPipe(outputChannel,(byte *) ServerCloseConnection , strlen(ServerCloseConnection)) < 0)
	{
		perror("closing server error");
		return -1;	
	}

	return 1;
	
}

int main(int argc, char ** argv)
{
	int inputChannel, outputChannel;
	serverState state = WAITING;
	char msg[2048];
	u_int16_t length;
	
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
		
		if(state == WAITING)
			fprintf(stderr , "-----------------------------------------------------------\nWaiting for connection\n");
		
		length = readFromPipe(inputChannel ,(byte *) msg);
		
		if(length == -1) //error in reading the message
			state = ERROR;
		
		if(strncmp(msg , ClientCloseConnection , length) == 0)	//if the next message is the client closing message
			state = CLOSING;
		
		switch(state){
			
			case WAITING :
				if(handshake(inputChannel , outputChannel , msg) == -1)	//handshake error
				{
					fprintf(stderr , "Handshake error \n");	
					state = ERROR;
				}
				else {
					state = HANDSHAKE;
					fprintf(stderr , "Handshake completed\n");
				}
			
			case HANDSHAKE : break;//call authenication
			
			case AUTHENTICATED : break; //call for key exchange
				
			case COMMUNICATION : break; //communication 
				
			case CLOSING	: 
				fprintf(stderr , "Client asked for closing, closing connection\n");
				
				closeConnection(inputChannel , outputChannel); 
				state = WAITING;
	
				fprintf(stderr , "Connection closed\n");
				
				break; //close connection
			
			case ERROR	 : 				
				fprintf(stderr , "Error during the communication, closing the connection\n");
				
				closeConnection(inputChannel , outputChannel); 
				state = WAITING;
	
				fprintf(stderr , "Connection closed\n");
				
				break; //close connection
				
			default : break;
		
		}


		//Authentication
		
		//key exange 
		
		//encrypted communication
		

	}while(1);
	
	
	



	exit(0);
}

