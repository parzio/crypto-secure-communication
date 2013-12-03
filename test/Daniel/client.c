#include "client.h"

int encrypt_and_send(byte * msg , const int length){
		
	if(encType == PLAIN)	//send plain text
	{
		if(writeInPipe(outputChannel, (byte *) msg , length) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}	
		return 0;
	}
	
	if(encType == RSA512)
	{
		BIGNUM *message = BN_new();			
		message = BN_bin2bn((const unsigned char *) msg, RSA512_BYTE_LENGTH , NULL);
		
		rsaEXP(message , &serverRsa);	//Encrypt with server RSA public key
		
		BN_bn2bin(message , msg);

		if(writeInPipe(outputChannel, (byte *) msg , RSA512_BYTE_LENGTH) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}			
		printf("%s\n", BN_bn2hex(message));
		BN_free(message);		
		return 0;
	}
	
	if(encType == RSA64)
	{
		BIGNUM *message = BN_new();			
		message = BN_bin2bn((const unsigned char *) msg, RSA64_BYTE_LENGTH , NULL);
		
		rsaEXP(message , &serverRsa);	//Encrypt with server RSA public key
				
		BN_bn2bin(message , msg);

		if(writeInPipe(outputChannel, (byte *) msg , RSA64_BYTE_LENGTH) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}
		
		printf("%s\n", BN_bn2hex(message));
		BN_free(message);		
		return 0;
	}
	
}

int receive_and_decrypt(byte * msg){

	int length = readFromPipe(inputChannel , msg); 
	
	if(length < 0)
	{
		fprintf(stderr , " **** error reading message **** \n\n");
		return -1;
	}	
	
	if(encType == PLAIN)
		return length;

	if(encType == RSA64)
	{
		BIGNUM *message = BN_new();	
		message = BN_bin2bn((const unsigned char *) msg, RSA64_BYTE_LENGTH , NULL);
		
		printf("%s\n", BN_bn2hex(message));
		
		rsaEXP(message , &clientRsa);	//Decrypt with private keyExchange
		
		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}
	
	if(encType == RSA512)
	{
		BIGNUM *message = BN_new();	
		message = BN_bin2bn((const unsigned char *) msg, RSA512_BYTE_LENGTH , NULL);
		
		printf("%s\n", BN_bn2hex(message));
		
		rsaEXP(message , &clientRsa);	//Decrypt with private keyExchange

		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}

}

int loadRsaKey(){
	
	serverRsa.modulo = BN_new();
	serverRsa.exponent = BN_new();
	
	readRsaKey(ServerRsa512PublicKeyFile , NULL , &serverRsa);
	
	clientRsa.modulo = BN_new();
	clientRsa.exponent = BN_new();
	
	readRsaKey(ClientRsa64PrivateKeyFile , clientName , &clientRsa); 	//obtain key

}

void clearRsaKey(){
	BN_free(serverRsa.modulo);
	BN_free(serverRsa.exponent);
	BN_free(clientRsa.modulo);
	BN_free(clientRsa.exponent);
}

int main(int argc, char ** argv){
	
	encType = PLAIN;
	
	/* Mandatory arguments */
	if( !argv[1] || !argv[2] || !argv[3] || !argv[4] ) {
		fprintf(stderr,"client [server->client fifo] [client->server fifo] [password file] [username]\n");
		exit(1);
	}

	/* Create connection with the server */
	fprintf(stderr,"Opening fifos...\n\n");
	outputChannel = openChannel(argv[2]);
	inputChannel  = openChannel(argv[1]);
	
	strcpy(clientName , argv[4]);
	
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
	
	fprintf(stderr , " **** Authentication phase **** \n\n");
	
	if(loadRsaKey() == -1)
	{
		fprintf(stderr , "Error while loading RSA keys\n" );
		return CLOSING;
	}

	BIGNUM *message = BN_new();
	BIGNUM *original = BN_new();

	bit  buffer[RSA512_BIT_LENGTH];	//buffer for the pseudo message
	byte msg512[RSA512_BYTE_LENGTH];		//pseudo message converted in byte
	
	memset(msg512 , 0 , RSA512_BYTE_LENGTH * sizeof(byte));
	
	initFPRNG();
	FPRNG(buffer, RSA512_BIT_LENGTH); // generates random bits.
	
	int i;
	
	for(i = 0; i < RSA512_BIT_LENGTH; i++)
		msg512[i/8] |= (buffer[i] << (i % 8));		//to byte

	original = BN_bin2bn((const unsigned char *) msg512, RSA512_BYTE_LENGTH , NULL);
	
	encType = RSA512;
	
	fprintf(stderr , "< Client challenge, server authentication > \n");
	encrypt_and_send(msg512 , RSA512_BYTE_LENGTH);
	
	encType = PLAIN;
	
	fprintf(stderr , "\n< Server response, server authentication >\n");
	receive_and_decrypt(msg512);
	
	message = BN_bin2bn((const unsigned char *) msg512, RSA512_BYTE_LENGTH , NULL);
	printf("%s\n", BN_bn2hex(original));
	
	fprintf(stderr , "\n< Client plain challenge, server authentication > \n");			
	printf("%s\n", BN_bn2hex(original));
	
	if(BN_cmp(original , message) != 0)
	{
		fprintf(stderr , "\n **** challenge Message error **** \n\n");	
		return CLOSING;
	}
	
	BN_free(message);
	BN_free(original);
	
	fprintf(stderr , "\n **** server authenticated  **** \n\n");	

	message = BN_new();
	
	encType = PLAIN;
	
	fprintf(stderr , "\n< Client name, client authentication >\n");
	encrypt_and_send(clientName , strlen(clientName));
	fprintf(stderr, "%s\n" , clientName);
	
	encType = RSA64;

	byte msg64[RSA64_BYTE_LENGTH];
		
	fprintf(stderr , "\n< Server challenge, client authentication >\n");
	receive_and_decrypt(msg64);
	
	encType = PLAIN;
	message = BN_bin2bn((const unsigned char *) msg64, RSA64_BYTE_LENGTH , NULL);
	
	fprintf(stderr , "\n< Client response , client authentication > \n");			
	encrypt_and_send(msg64 , RSA64_BYTE_LENGTH);
	printf("%s\n", BN_bn2hex(message));
		
	BN_free(message);
		
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
	clearRsaKey();
	
}
