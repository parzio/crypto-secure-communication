
#include "server.h"

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
		
		rsaEXP(message , &clientRsa);	//Encrypt with server RSA public key
		
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
		
		rsaEXP(message , &clientRsa);	//Encrypt with server RSA public key
				
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
		
		rsaEXP(message , &serverRsa);	//Decrypt with private keyExchange
		
		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}
	
	if(encType == RSA512)
	{
		BIGNUM *message = BN_new();	
		message = BN_bin2bn((const unsigned char *) msg, RSA512_BYTE_LENGTH , NULL);
		
		printf("%s\n", BN_bn2hex(message));
		
		rsaEXP(message , &serverRsa);	//Decrypt with private keyExchange

		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}

}

int loadServerRsaKey(){	
	serverRsa.modulo = BN_new();
	serverRsa.exponent = BN_new();	
	readRsaKey(ServerRsa512PrivateKeyFile , NULL , &serverRsa);
}

int loadClientRsaKey(const char * clientName){
	clientRsa.modulo = BN_new();
	clientRsa.exponent = BN_new();	
	readRsaKey(ClientRsa64PublicKeyFile , clientName , &clientRsa); 	//obtain key
}

void clearRsaKey(){
	BN_free(serverRsa.modulo);
	BN_free(serverRsa.exponent);
	BN_free(clientRsa.modulo);
	BN_free(clientRsa.exponent);
}

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
	
	fprintf(stderr , " **** Authentication phase **** \n\n");
	
	if(loadServerRsaKey() == -1)
	{
		fprintf(stderr , "Error while loading RSA keys\n" );
		return ERROR;
	}
	
	BIGNUM *message = BN_new();

	byte msg512[RSA512_BYTE_LENGTH];

	encType = RSA512;
	
	fprintf(stderr , "< Client challenge, server authentication > \n");
	receive_and_decrypt(msg512);
	
	encType = PLAIN;
	
	fprintf(stderr , "\n< Server response, server authentication >\n");
	encrypt_and_send(msg512 , RSA512_BYTE_LENGTH);	
	
	message = BN_bin2bn((const unsigned char *) msg512, RSA512_BYTE_LENGTH , NULL);
	printf("%s\n", BN_bn2hex(message));
	
	BN_free(message);
			
	fprintf(stderr , "\n **** client authentication **** \n\n");
	
	char clientName[256];
	
	fprintf(stderr , "\n< Client name , client authentication >\n");
	int length = receive_and_decrypt(clientName);	
	
	clientName[length] = '\0';
	
	fprintf(stderr , "%s\n" , clientName);		
	
	if(loadClientRsaKey(clientName) == -1)
	{
		fprintf(stderr , "Error while loading RSA keys\n" );
		return ERROR;
	}
	
	
	message = BN_new();
	BIGNUM *original = BN_new();

	bit  buffer[RSA64_BIT_LENGTH];	//buffer for the pseudo message
	byte msg64[RSA64_BYTE_LENGTH];		//pseudo message converted in byte
	
	memset(msg64 , 0 , RSA64_BYTE_LENGTH * sizeof(byte));
	
	initFPRNG();
	FPRNG(buffer, RSA64_BIT_LENGTH); // generates random bits.
	
	int i;
	
	for(i = 0; i < RSA64_BIT_LENGTH; i++)
		msg64[i/8] |= (buffer[i] << (i % 8));		//to byte
				
	original = BN_bin2bn((const unsigned char *) msg64, RSA64_BYTE_LENGTH , NULL);
	
	encType = RSA64;
	
	fprintf(stderr , "< Server challenge, client authentication > \n");
	encrypt_and_send(msg64 , RSA64_BYTE_LENGTH);

	encType = PLAIN;
	
	fprintf(stderr , "\n< Client response, client authentication >\n");
	receive_and_decrypt(msg64);

	message = BN_bin2bn((const unsigned char *) msg64, RSA64_BYTE_LENGTH , NULL);		
	printf("%s\n", BN_bn2hex(message));
	
	fprintf(stderr , "\n< Client plain challenge, server authentication > \n");		
	printf("%s\n", BN_bn2hex(original));
	
	int equal = BN_cmp(message , original);
	
	BN_free(clientRsa.modulo);
	BN_free(clientRsa.exponent);
	BN_free(message);
	BN_free(original);
	
	if(equal != 0)
	{
		fprintf(stderr , "\n **** challenge Message error **** \n\n");	
		return ERROR;
	}
	
	fprintf(stderr , "\n **** Authentication phase completed **** \n\n");
	
	return AUTHENTICATED;
	
}

serverState keyExchange(){
	
	return COMMUNICATION;
	
}

serverState communication(){	
	
	char buffer[MSG_MAX_SIZE];
	
	int length = 0;
	
		FILE *file = fopen(ServerCommunicationFile, "w+");
	
		if(file == NULL){
			perror("Server txt file ");
			return ERROR;
		}

		do{
			
		length = readFromPipe(inputChannel ,(byte *) buffer);
		
		if(length == -1)
		{
			fclose(file);
			return ERROR;
		}
		
		//decrypt
		
		printMsg("Client to server ---> " , buffer , length);

		if(strncmp(buffer , ClientCloseConnection , length) == 0 && length == strlen(ClientCloseConnection)){
			fclose(file);
			return CLOSING;
		}
		
		fprintf(file ,"%s" , (const char *) buffer);
		
		char response[MSG_MAX_SIZE];
		
		strcpy(response , "ACK MSG : ");
		strncat(response , buffer , length);
		int res_length = strlen("ACK MSG : ") + length;
		
		//encrypt

		if(writeInPipe(outputChannel,(byte *) response , res_length) < 0)
		{
			fprintf(stderr , "\n **** communication write error **** \n\n");
			fclose(file);
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
	
	state = WAITING;

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

