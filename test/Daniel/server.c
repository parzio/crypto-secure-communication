
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
	
	if(encType == RSA512){
		BIGNUM *message = BN_new();	
		
		byte fullMsg[RSA512_BYTE_LENGTH];
		
		memset(fullMsg , 0 , sizeof(byte) * RSA512_BYTE_LENGTH);
		memcpy(fullMsg , msg , sizeof(byte) * length);
		
		message = BN_bin2bn((const unsigned char *) fullMsg, RSA512_BYTE_LENGTH , NULL);
		
		rsaEXP(message , &client512Rsa);	//Encrypt with server RSA public key
		
		BN_bn2bin(message , fullMsg);

		if(writeInPipe(outputChannel, (byte *) fullMsg , RSA512_BYTE_LENGTH) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}
		
		printf("RSA512 : %s\n", BN_bn2hex(message));
		BN_free(message);		
		return 0;
	}
	
	if(encType == RSA64){
		BIGNUM *message = BN_new();
		
		byte fullMsg[RSA64_BYTE_LENGTH];
		
		memset(fullMsg , 0 , sizeof(byte) * RSA64_BYTE_LENGTH);
		memcpy(fullMsg , msg , sizeof(byte) * length);
		
		message = BN_bin2bn((const unsigned char *) fullMsg, RSA64_BYTE_LENGTH , NULL);
		
		rsaEXP(message , &client64Rsa);	//Encrypt with server RSA public key
				
		BN_bn2bin(message , fullMsg);

		if(writeInPipe(outputChannel, (byte *) fullMsg , RSA64_BYTE_LENGTH) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}
		
		printf("RSA64 : %s\n", BN_bn2hex(message));
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

	if(strncmp(msg , ClientCloseConnection , length) == 0 && length == strlen(ClientCloseConnection))
		return length;
	
	if(encType == PLAIN)
		return length;

	if(encType == RSA64){
		BIGNUM *message = BN_new();	
		message = BN_bin2bn((const unsigned char *) msg, RSA64_BYTE_LENGTH , NULL);
		
		printf("%s\n", BN_bn2hex(message));
		
		rsaEXP(message , &server64Rsa);	//Decrypt with private keyExchange
		
		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}
	
	if(encType == RSA512){
		BIGNUM *message = BN_new();	
		message = BN_bin2bn((const unsigned char *) msg, RSA512_BYTE_LENGTH , NULL);
		
		printf("%s\n", BN_bn2hex(message));
		
		rsaEXP(message , &server512Rsa);	//Decrypt with private keyExchange

		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}
	
	if(encType == Cipher_ALL5){
		BIGNUM *tm = BN_new();	
		tm = BN_bin2bn((const unsigned char *) msg, length , NULL);
		printf("ALL5 : %s\n", BN_bn2hex(tm));

		byte message[length - HASH_BYTE_LENGTH];	
		byte hash[HASH_BYTE_LENGTH];

		int recv_length = length;
		
		if(cipherSpec.hash_function == hash_spongeBunny)
		{
		recv_length -= HASH_BYTE_LENGTH;	
		memmove(hash , msg + (sizeof(byte) * recv_length) , sizeof(byte) * HASH_BYTE_LENGTH);	//copy the hash
		memcpy(message , msg , sizeof(byte) * recv_length);	//copy the cihpertext				
		
		ALL5_decrypt(&cipherStruct.all5, message, message, recv_length);	//decrypt
		
		byte rec_hash[HASH_BYTE_LENGTH];
		spongeBunnyComputeHash(message , rec_hash , recv_length);	//compute the hash of the plaintext
		
		if(memcmp(rec_hash, hash, sizeof(byte) * HASH_BYTE_LENGTH) != 0)
		{
			recv_length = 0;
			fprintf(stderr , "Wrong HASH !!!!!!! \n");
		}
		
		tm = BN_bin2bn((const unsigned char *) hash, HASH_BYTE_LENGTH , NULL);
		printf("HASH : %s\n", BN_bn2hex(tm));
		memcpy(msg , message , sizeof(byte) * recv_length);
		
			
		}	//no hash function
		else
			ALL5_decrypt(&cipherStruct.all5, msg, msg, recv_length);	//decrypt	
		
		BN_free(tm);		
		return recv_length;
	}
	
	if(encType == Cipher_MAJ5){
		BIGNUM *tm = BN_new();	
		tm = BN_bin2bn((const unsigned char *) msg, length , NULL);
		printf("MAJ5 : %s\n", BN_bn2hex(tm));
		
		int recv_length = length;
		
		if(cipherSpec.hash_function == hash_spongeBunny)		
		{
		byte message[length - HASH_BYTE_LENGTH];	
		byte hash[HASH_BYTE_LENGTH];
		
		recv_length -= HASH_BYTE_LENGTH;	
		memmove(hash , msg + (sizeof(byte) * recv_length) , sizeof(byte) * HASH_BYTE_LENGTH);	//copy the hash
		memcpy(message , msg , sizeof(byte) * recv_length);	//copy the cihpertext				
		
		MAJ5_decrypt(&cipherStruct.maj5, message, message, recv_length);	//decrypt
		
		byte rec_hash[HASH_BYTE_LENGTH];
		spongeBunnyComputeHash(message , rec_hash , recv_length);	//compute the hash of the plaintext
		
		if(memcmp(rec_hash, hash, sizeof(byte) * HASH_BYTE_LENGTH) != 0)
		{
			recv_length = 0;
			fprintf(stderr , "Wrong HASH !!!!!!! \n");
		}
		
		tm = BN_bin2bn((const unsigned char *) hash, HASH_BYTE_LENGTH , NULL);
		printf("HASH : %s\n", BN_bn2hex(tm));
		memcpy(msg , message , sizeof(byte) * recv_length);
		
		}	//no hash function
		else
			MAJ5_decrypt(&cipherStruct.maj5, msg, msg, recv_length);	//decrypt	
		
		BN_free(tm);		
		return recv_length;
		
	}
	
	if(encType == Cipher_Bunny24){
		
		BIGNUM *tm = BN_new();	
		tm = BN_bin2bn((const unsigned char *) msg, length , NULL);
		printf("BUNNY24 : %s \n", BN_bn2hex(tm));
		
		int recv_length = length - HASH_BYTE_LENGTH;
		
		byte hash[HASH_BYTE_LENGTH];	
		byte mess[recv_length];
		
		memmove(hash , msg + (sizeof(byte) * recv_length) , sizeof(byte) * HASH_BYTE_LENGTH);	//copy the hash					
		memcpy(mess , msg , sizeof(byte) * recv_length);
		
		int message_bit_length = recv_length * 8;
		
		bit  plain_bit_text[message_bit_length];			//plaintext in  bits
		bit  crypt_bit_text[message_bit_length];			//ciphertext in bits
		bit  plain_byte_text[recv_length];
		
		memset(plain_byte_text , 0 , sizeof(byte) * recv_length);

		int i;

		for(i = 0; i < message_bit_length; i++)
			crypt_bit_text[i] = (msg[i / 8] & (1 << i % 8)) ? 1 : 0;		//to bit
			
		bunny24CBC_decrypt(plain_bit_text, crypt_bit_text , init_vector , cipherStruct.key , message_bit_length); // encrypt

		for(i = 0; i < message_bit_length; i++)
			plain_byte_text[i/8] |= (plain_bit_text[i] << (i % 8));		//to byte
			
		strcpy(msg ,plain_byte_text);
		msg[strlen(msg) + 1] = '\0';

		byte rec_hash[HASH_BYTE_LENGTH];
		memset(rec_hash , 0 , sizeof(byte) * HASH_BYTE_LENGTH);
		spongeBunnyComputeHash(msg , rec_hash , strlen(msg) + 1);	//compute the hash of the ciphertext
		
		if(memcmp(rec_hash, hash, sizeof(byte) * HASH_BYTE_LENGTH) != 0)
			fprintf(stderr , "Wrong HASH !!!!!!! \n");	
		
		tm = BN_bin2bn((const unsigned char *) rec_hash , HASH_BYTE_LENGTH , NULL);
		printf("\nHASH : %s\n", BN_bn2hex(tm));

		BN_free(tm);	
		
		return strlen(msg) + 1;
	}
	
}

int loadServerRsaKey(){	

	server512Rsa.modulo 	 = BN_new();
	server512Rsa.exponent = BN_new();
	
	server64Rsa.modulo = BN_new();
	server64Rsa.exponent = BN_new();
	
	readRsaKey(ServerRsa512PrivateKeyFile , NULL , &server512Rsa);

	readRsaKey(ServerRsa64PrivateKeyFile , NULL , &server64Rsa);

}

int loadClientRsaKey(const char * clientName){

	client512Rsa.modulo = BN_new();
	client512Rsa.exponent = BN_new();
	
	client64Rsa.modulo = BN_new();
	client64Rsa.exponent = BN_new();

	readRsaKey(ClientRsa64PublicKeyFile , clientName , &client64Rsa); 	

	readRsaKey(ClientRsa512PublicKeyFile , clientName , &client512Rsa); 
}

void clearRsaKey(){
	BN_free(server512Rsa.modulo);
	BN_free(server512Rsa.exponent);
	BN_free(client64Rsa.modulo);
	BN_free(client64Rsa.exponent);
	BN_free(server64Rsa.modulo);
	BN_free(server64Rsa.exponent);
	BN_free(client512Rsa.modulo);
	BN_free(client512Rsa.exponent);
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
	
	fprintf(stderr , "\n< Server plain challenge, client authentication > \n");		
	printf("%s\n", BN_bn2hex(original));
	
	if(BN_cmp(original , message) != 0)
	{
		fprintf(stderr , "\n **** challenge Message error **** \n\n");	
		return CLOSING;
	}
	
	BN_free(message);
	BN_free(original);
	
	fprintf(stderr , "\n **** Authentication phase completed **** \n\n");
	
	return AUTHENTICATED;
	
}

serverState keyExchange(){
	
	encType = PLAIN;
	
	fprintf(stderr , " **** Key Exhange phase **** \n\n");
	
	char cChiper[1];
	
	fprintf(stderr , "\n< Client cipher spec , client key exchange >\n");
	receive_and_decrypt(cChiper);
	fprintf(stderr , " %c \n" , cChiper[0]);
	
	if(isInCipherSpec(ServerCipherSpecFile , cChiper) == -1)	//if i have the cipher
	{
		fprintf(stderr , "\n< No cipher %c >\n" , cChiper[0]);
		return ERROR;
	}
	
	fprintf(stderr , "\n< Client cipher accepted , key generation >\n");	
	switch(cChiper[0]){
		
		case 'A': cipherSpec.symmetric_cipher = Cipher_Bunny24; 
					 cipherSpec.public_cipher = RSA64;
					 cipherSpec.hash_function = hash_spongeBunny;
					 break;
					 
		case 'B': cipherSpec.symmetric_cipher = Cipher_Bunny24; 
					 cipherSpec.public_cipher = RSA512;
					 cipherSpec.hash_function = hash_spongeBunny;
					 break;
					 
		case 'C': cipherSpec.symmetric_cipher = Cipher_ALL5; 
					 cipherSpec.public_cipher = RSA64;
					 cipherSpec.hash_function = hash_spongeBunny;
					 break;
					 
		case 'D': cipherSpec.symmetric_cipher = Cipher_ALL5; 
					 cipherSpec.public_cipher = RSA512;
					 cipherSpec.hash_function = hash_spongeBunny;
					 break;
					 
		case 'E': cipherSpec.symmetric_cipher = Cipher_MAJ5; 
					 cipherSpec.public_cipher = RSA64;
					 cipherSpec.hash_function = hash_spongeBunny;
					 break;
					 
		case 'F': cipherSpec.symmetric_cipher = Cipher_MAJ5; 
					 cipherSpec.public_cipher = RSA512;
					 cipherSpec.hash_function = hash_spongeBunny;
					 break;
					 
		default : break;
	}
	
	bit buffer24[24] , buffer64[64];
	byte key24[3] , key64[8];
	
	memset(key24 , 0 , sizeof(byte) * 3);
	memset(key64 , 0 , sizeof(byte) * 8);
	
	int i;
	BIGNUM *tm;
	tm = BN_new();
	
	encType = cipherSpec.public_cipher;

	if(cipherSpec.symmetric_cipher == Cipher_Bunny24)
	{
		FPRNG(buffer24 , 24);
		for(i = 0; i < 24; i++)
			key24[i/8] |= (buffer24[i] << (i % 8));		//to byte
			
		tm = BN_bin2bn((const unsigned char *) key24, 3 , NULL);	
		printf("PLAIN : %s\n", BN_bn2hex(tm));	
		encrypt_and_send(key24 , 3);
		cipherInit(&cipherStruct , buffer24 , 24 , init_vector , 24, Cipher_Bunny24);
	}
	else
	{
		FPRNG(buffer64 , 64);
		for(i = 0; i < 64; i++)
			key64[i/8] |= (buffer64[i] << (i % 8));		//to byte		
			
		tm = BN_bin2bn((const unsigned char *) key64, 8 , NULL);	
		printf("PLAIN  : %s\n", BN_bn2hex(tm));	
		encrypt_and_send(key64 , 8);	
		cipherInit(&cipherStruct , buffer64 , 64 , init_vector , 24, cipherSpec.symmetric_cipher);
	}
	
	BN_free(tm);

	fprintf(stderr , "\n **** Key Exhange completed **** \n\n");
	
	return COMMUNICATION;
	
}

serverState communication(){	
	
	char msg[MSG_MAX_SIZE];
	
	int length = 0;

	do{

	encType = cipherSpec.symmetric_cipher;	
		
	fprintf(stderr , "\n< Client message , communication > \n");			
	length = receive_and_decrypt(msg);	

	if(strncmp(msg , ClientCloseConnection , length) == 0 && length == strlen(ClientCloseConnection)){
		fprintf(stderr , "%s" , ClientCloseConnection);
		encType = PLAIN;
		return CLOSING;
	}
	encType = PLAIN;
		
	fprintf(stderr , "\n< Server response , communication >\n" );			
	encrypt_and_send(msg , length);	
	printMsg("" , msg ,(u_int16_t) length);
	
	}while(1);
		
}

serverState closeConnection(){
	
	encType = PLAIN;
	
	fprintf(stderr , "\n< Server response , closing >\n" );	
	encrypt_and_send(ServerCloseConnection , strlen(ServerCloseConnection));
	fprintf(stderr , "%s" , ServerCloseConnection);

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

