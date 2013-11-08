
#include "server.h"

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

serverState handshake(){

	fprintf(stderr , "-----------------------------------------------------------\n **** Waiting for handshake ****\n\n");	
	
	char buffer[MSG_MAX_SIZE];
	
	int length = readFromPipe(inputChannel ,(byte *) buffer);
				
	printMsg("Client to server ---> " , buffer , length);	
	
	if(strncmp(buffer , ClientOpenConnection , length) != 0 || length != strlen(ClientOpenConnection))
	{
		fprintf(stderr, "Handshake read error\n");
		return ERROR;
	}
		
	if(writeInPipe(outputChannel,(byte *) ServerOpenConnection , strlen(ServerOpenConnection)) < 0)
	{
		fprintf(stderr, "Handshake write error\n");
		return ERROR;
	}
	
	printMsg("Server to client ---> " , ServerOpenConnection , strlen(ServerOpenConnection));
	
	return HANDSHAKE;
}

serverState authentication(){
	
	return AUTHENTICATED;
	
}

serverState keyExchange(){
	
	return COMMUNICATION;
	
}

serverState communication(){	
	
	char buffer[MSG_MAX_SIZE];
	
	int length = 0;

		do{
			
		length = readFromPipe(inputChannel ,(byte *) buffer);
		
		if(length == -1)
			return ERROR;
			
		//decrypt
		
		printMsg("Client to server ---> " , buffer , length);	
			
		if(strncmp(buffer , ClientCloseConnection , length) == 0 && length == strlen(ClientCloseConnection))
			return CLOSING;
		
		char response[MSG_MAX_SIZE];
		
		strcpy(response , "ACK MSG : ");
		strncat(response , buffer , length);
		int res_length = strlen("ACK MSG : ") + length;

		if(writeInPipe(outputChannel,(byte *) response , res_length) < 0)
		{
			fprintf(stderr , "\n **** communication write error **** \n\n");
			return ERROR;	
		}
		
		printMsg("Server to client ---> " , response , res_length);	
		fprintf(stderr , "\n");
			
		}while(1);
		
}

serverState closeConnection(){
	
	if(writeInPipe(outputChannel,(byte *) ServerCloseConnection , strlen(ServerCloseConnection)) < 0)
	{
		fprintf(stderr , "\n **** closing falied **** \n\n");
		return ERROR;	
	}
	
	printMsg("Server to client ---> " , ServerOpenConnection , strlen(ServerOpenConnection));
					
	return WAITING;
	
}

int main(int argc, char ** argv){
	
	serverState state = WAITING;

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

		switch(state){
			
			case WAITING : state = handshake();

				if(state == ERROR)	//handshake error
					fprintf(stderr , "\n **** handshake falied **** \n\n");
				else 
					fprintf(stderr , "\n **** handshake succeded **** \n\n");
				break;
			
			case HANDSHAKE : state = authentication(); 
				
				break;
			
			case AUTHENTICATED : state = keyExchange();
				
				break;
				
			case COMMUNICATION : state = communication();
			
				break;
				
			case CLOSING	: state = closeConnection(); 
			
				outputChannel = openFifo(argv[1]);
				inputChannel = openFifo(argv[2]);
	
				if(state == ERROR)	//handshake error
					fprintf(stderr , "\n **** closing falied **** \n\n");
				else 
					fprintf(stderr , "\n **** closing succeded **** \n\n");
				break;

			case ERROR	 :
				
				fprintf(stderr , "\nError during the communication, force shutodown of the client\n\n");
				
				closeConnection(); 
				state = WAITING;
				
				outputChannel = openFifo(argv[1]);
				inputChannel = openFifo(argv[2]);
				
				fprintf(stderr , "\n **** closing succeded **** \n\n");
				
				break; //close connection
				
			default : break;
		
		}

	}while(1);
	

	exit(0);
}

