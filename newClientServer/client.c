#include "client.h"

int main(int argc, char ** argv){
	
	clientState state;
	
	/* Mandatory arguments */
	if( !argv[1] || !argv[2] || !argv[3] || !argv[4] ) {
		fprintf(stderr,"client [server->client fifo] [client->server fifo] [password file] [username]\n");
		exit(1);
	}

	/* Create connection with the server */
	fprintf(stderr,"Opening fifos...\n\n");
	outputChannel = openChannel(argv[2]);
	inputChannel  = openChannel(argv[1]);
	
	state = handshake();
	
	if(state == SHUTDOWN) //handshake error
	{
		fprintf(stderr , "\n **** handshake falied **** \n\n");
		fprintf(stderr , "\n **** closing connection **** \n\n");	
		exit(0);
	}
	else
		fprintf(stderr , "\n **** handshake succeded **** \n\n");
	
	
	state = authentication();

	if(state == SHUTDOWN) //authentication error
	{
		fprintf(stderr , "\n **** authentication falied **** \n\n");
		fprintf(stderr , "\n **** closing connection **** \n\n");	
		exit(0);
	}
	else
		fprintf(stderr , "\n **** authentication succeded **** \n\n");
	
	state = keyExchange();
	
	state = communication();

	if(state == SHUTDOWN) //authentication error
	{
		fprintf(stderr , "\n **** communication falied **** \n\n");
		fprintf(stderr , "\n **** closing connection **** \n\n");	
		exit(0);
	}
	
	closeConnection();
	
	return 0;
}

clientState handshake(){
	
	char msg[MSG_MAX_SIZE];
	
	fprintf(stderr , " **** handshake phase **** \n\n");
	
	if(writeInPipe(outputChannel, (byte *) ClientOpenConnection , strlen(ClientOpenConnection)) < 0)
	{
		fprintf(stderr, "handshake write error \n");
		return SHUTDOWN;	
	}
	
	printMsg("Client to server ---> " , ClientOpenConnection , strlen(ClientOpenConnection));

	readFromPipe(inputChannel ,(byte *) msg);
	
	if(strncmp(msg , ServerOpenConnection , strlen(ServerOpenConnection)) != 0)
	{
		fprintf(stderr, "handshake read error \n");
		return SHUTDOWN;
	}
	
	printMsg("Server to client ---> " , ServerOpenConnection , strlen(ServerOpenConnection));
		
	return HANDSHAKE;
}

clientState authentication(){
	return AUTHENTICATED;
}

clientState keyExchange(){
	return COMMUNICATION;
}

clientState communication(){
	
	char msg[MSG_MAX_SIZE];
	int msgLength = 0;
	
	FILE *file = fopen(ClientCommunicationFile, "rt");
	
	if(file == NULL){
		perror("Client txt file ");
		return CLOSING;
	}
	
	fprintf(stderr , " **** Communication phase **** \n\n");
	
	do{
	
	msgLength = readFromFile(file , msg);
	
	if(msgLength == -1)
	{	
		fprintf(stderr , "\n **** Communication phase ended **** \n\n");
		return CLOSING;
	}
	
	//Encrypt msg 
	
	if(writeInPipe(outputChannel, (byte *) msg , msgLength) < 0)
	{
		fprintf(stderr , " **** Communication phase write error **** \n\n");
		return CLOSING;	
	}

	printMsg("Client to server ---> " ,msg ,(u_int16_t) msgLength);
	
	msgLength = readFromPipe(inputChannel ,(byte *) msg);

	if(msgLength < 0)
	{
		fprintf(stderr , " **** Communication phase read error **** \n\n");
		return CLOSING;
	}
	
	//decrypt
	
	if(strncmp(msg , ServerCloseConnection , strlen(ServerCloseConnection)) == 0)	
		return SHUTDOWN;
	
	printMsg("Server to client ---> " ,msg ,(u_int16_t) msgLength);
	
	fprintf(stderr , "\n");

	}while(1);
	
	return SHUTDOWN;
	
}

void closeConnection(){
	
	char msg[MSG_MAX_SIZE];
	
	fprintf(stderr,"\n **** Closing phase **** \n\n");

	if(writeInPipe(outputChannel, (byte *) ClientCloseConnection , strlen(ClientCloseConnection)) < 0)
	{
		fprintf(stderr,"Closing write error \n");
		return;	
	}
	
	printMsg("Client to server ---> " , ClientCloseConnection , strlen(ClientCloseConnection));
	
	readFromPipe(inputChannel ,(byte *) msg);
	
	if(strncmp(msg , ServerCloseConnection , strlen(ServerCloseConnection)) != 0)
	{
		fprintf(stderr,"Closing read error \n");
		return;
	}
	printMsg("Server to client ---> " , ServerCloseConnection , strlen(ServerCloseConnection));
	
	fprintf(stderr,"\n **** Closing succeded **** \n\n");

	closeChannel(inputChannel);
	closeChannel(outputChannel);

}
