#include "common.h"

typedef enum clientState {
	HANDSHAKE , 		//Handshake completed, authenication
	AUTHENTICATED ,   //Authentication done , key Exchange phase
	COMMUNICATION , 	//Key and protocol established, communication
	CLOSING				//Send close message, wait for response and close
} clientState;

int askConnection(int inputChannel, int outputChannel){
	
	char msg[2048];
	
	fprintf(stderr,"Asking for connection\n");
	
	if(writeInPipe(outputChannel, (byte *) ClientOpenConnection , strlen(ClientOpenConnection)) < 0)
	{
		perror("handshake client error");
		return -1;	
	}

	readFromPipe(inputChannel ,(byte *) msg);
	
	if(strncmp(msg , ServerOpenConnection , strlen(ServerOpenConnection)) != 0)
	{
		perror("handshake client error");
		return -1;
	}
		
	return 1;
}


void closeConnection(int inputChannel , int outputChannel){
	
	char msg[2048];
	
	fprintf(stderr,"Sending closing connection message : %s\n" , ClientCloseConnection);
	
	if(writeInPipe(outputChannel, (byte *) ClientCloseConnection , strlen(ClientCloseConnection)) < 0)
	{
		perror("Closing client error");
		return;	
	}
	
	fprintf(stderr , "message written , waiting for response\n");
	
	readFromPipe(inputChannel ,(byte *) msg);
	
	if(strncmp(msg , ServerCloseConnection , strlen(ServerCloseConnection)) != 0)
	{
		perror("Closing client error");
		return;
	}
	
	fprintf(stderr , "Connection closed\n\n");
	
	closeChannel(inputChannel);
	closeChannel(outputChannel);

}

int main(int argc, char ** argv)
{
	int inputChannel, outputChannel;
	clientState state;
	//char msg[2048];
	//u_int16_t length;

	/* Mandatory arguments */
	if( !argv[1] || !argv[2] || !argv[3] || !argv[4] ) {
		fprintf(stderr,"client [server->client fifo] [client->server fifo] [password file] [username]\n");
		exit(1);
	}

	/* Create connection with the server */
	fprintf(stderr,"Create connection...\n");
	outputChannel = openChannel(argv[2]);
	inputChannel  = openChannel(argv[1]);
	
	
	if(askConnection(inputChannel , outputChannel) == -1) //handshake error
	{
		fprintf(stderr , "handshake falied \n");
		state = CLOSING;	
	}
	else
	{
		fprintf(stderr , "handshake succeded \n");
		state = CLOSING;
	}
	
	switch(state){
			
			case HANDSHAKE : break;//call authenication
			
			case AUTHENTICATED : break; //call for key exchange
				
			case COMMUNICATION : break; //communication 
				
			case CLOSING : 
				closeConnection(inputChannel , outputChannel); 
				break;
				
			default : break;
	}
	
	return 0;
}
