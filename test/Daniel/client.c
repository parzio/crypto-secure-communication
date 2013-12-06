#include "client.h"

int encrypt_and_send(byte * msg ,  int length){
		
	if(encType == PLAIN){
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
		
		rsaEXP(message , &server512Rsa);	//Encrypt with server RSA public key
		
		BN_bn2bin(message , fullMsg);

		if(writeInPipe(outputChannel, (byte *) fullMsg , RSA512_BYTE_LENGTH) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}			
		printf("RSA 512 : %s\n", BN_bn2hex(message));
		BN_free(message);		
		return 0;
	}
	
	if(encType == RSA64){
		BIGNUM *message = BN_new();	

		byte fullMsg[RSA64_BYTE_LENGTH];
		
		memset(fullMsg , 0 , sizeof(byte) * RSA64_BYTE_LENGTH);
		memcpy(fullMsg , msg , sizeof(byte) * length);
		
		message = BN_bin2bn((const unsigned char *) fullMsg, RSA64_BYTE_LENGTH , NULL);
		
		rsaEXP(message , &server64Rsa);	//Encrypt with server RSA public key
				
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
	
	if(encType == Cipher_Bunny24){
		BIGNUM *tm = BN_new();	

		if(msg[length -1] != '\0')
		{
			length ++;
			msg[length - 1] = '\0';
		}

		int message_bit_length = length * 8;				//bits of the msg
		int ciphertext_bit_length = getCipherLength(message_bit_length);
		
		int ciphertext_byte_length = ciphertext_bit_length / 8;
		
		bit  plain_bit_text[message_bit_length];				//plaintext in  bits
		bit  crypt_bit_text[ciphertext_bit_length];			//ciphertext in bits		
		byte crypt_byte_text[ciphertext_byte_length];		//ciphertext in bytes
	
		memset(crypt_byte_text , 0 , ciphertext_byte_length * sizeof(byte));	//init
		
		byte hash[HASH_BYTE_LENGTH];
		memset(hash , 0 , sizeof(byte) * HASH_BYTE_LENGTH);
		if(cipherSpec.hash_function == hash_spongeBunny)
			spongeBunnyComputeHash(msg , hash , length);	//compute the hash of the plaintext
			
		int i;
		
		for(i = 0; i < message_bit_length; i++)
			plain_bit_text[i] = (msg[i / 8] & (1 << i % 8)) ? 1 : 0;		//to bit

		bunny24CBC_encrypt(plain_bit_text, crypt_bit_text , init_vector , cipherStruct.key , message_bit_length); // encrypt
				
		for(i = 0; i < ciphertext_bit_length; i++)
			crypt_byte_text[i/8] |= (crypt_bit_text[i] << (i % 8));		//to byte*/
		
		int send_length = ciphertext_byte_length;

		byte fullMsg[ciphertext_byte_length + HASH_BYTE_LENGTH];	//message + hash_function
		memcpy(fullMsg , crypt_byte_text , sizeof(byte) * ciphertext_byte_length);
		
		
		if(cipherSpec.hash_function == hash_spongeBunny)
		{
			memmove(fullMsg + (sizeof(byte) * ciphertext_byte_length) , hash , sizeof(byte) * HASH_BYTE_LENGTH);	//append the hash	
			send_length += HASH_BYTE_LENGTH; 	//inc msg length
		}

		if(writeInPipe(outputChannel, (byte *) fullMsg , send_length) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}

		tm = BN_bin2bn((const unsigned char *) fullMsg, send_length , NULL);
		printf("BUNNY24 : %s\n", BN_bn2hex(tm));
		BN_free(tm);			
		return 0;
	}
	
	if(encType == Cipher_ALL5){
		BIGNUM *tm = BN_new();	

		byte ciphertext[length];
		
		byte hash[HASH_BYTE_LENGTH];
		
		if(cipherSpec.hash_function == hash_spongeBunny)
			spongeBunnyComputeHash(msg , hash , length);	//compute the hash of the plaintext
				
		ALL5_encrypt(&cipherStruct.all5, msg, ciphertext, length);	//encrypt the plaintext
		
		int send_length = length;
		byte fullMsg[length + HASH_BYTE_LENGTH];	//message + hash_function
		memcpy(fullMsg , ciphertext , sizeof(byte) * length);
		
		if(cipherSpec.hash_function == hash_spongeBunny)
		{
			memmove(fullMsg + (sizeof(byte) * length) , hash , sizeof(byte) * HASH_BYTE_LENGTH);	//append the hash	
			send_length += HASH_BYTE_LENGTH; 	//inc msg length
		}

		if(writeInPipe(outputChannel, (byte *) fullMsg , send_length) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}
		
		tm = BN_bin2bn((const unsigned char *) fullMsg, length , NULL);
		printf("ALL5 : %s\n", BN_bn2hex(tm));
		BN_free(tm);	
		
	}
	
	if(encType == Cipher_MAJ5){
		BIGNUM *tm = BN_new();	

		byte ciphertext[length];
		byte hash[HASH_BYTE_LENGTH];
		
		if(cipherSpec.hash_function == hash_spongeBunny)
			spongeBunnyComputeHash(msg , hash , length);	//compute the hash of the plaintext
			
		MAJ5_encrypt(&cipherStruct.maj5, msg, ciphertext, length);
		
		int send_length = length;
		byte fullMsg[length + HASH_BYTE_LENGTH];	//message + hash_function
		memcpy(fullMsg , ciphertext , sizeof(byte) * length);
		
		if(cipherSpec.hash_function == hash_spongeBunny)
		{
			memmove(fullMsg + (sizeof(byte) * length) , hash , sizeof(byte) * HASH_BYTE_LENGTH);	//append the hash	
			send_length += HASH_BYTE_LENGTH; 	//inc msg length
		}
		
		if(writeInPipe(outputChannel, (byte *) fullMsg , send_length) < 0)
		{
			fprintf(stderr , " **** Communication phase write error **** \n\n");
			return -1;	
		}
		
		tm = BN_bin2bn((const unsigned char *) fullMsg, send_length , NULL);
		printf("MAJ5 : %s\n", BN_bn2hex(tm));
		BN_free(tm);			
	}
	
}

int receive_and_decrypt(byte * msg){

	int length = readFromPipe(inputChannel , msg); 
	
	if(length < 0){
		fprintf(stderr , " **** error reading message **** \n\n");
		return -1;
	}	
	
	if(encType == PLAIN)
		return length;

	if(encType == RSA64){
		BIGNUM *message = BN_new();

		message = BN_bin2bn((const unsigned char *) msg, length , NULL);
		
		printf("RSA64 : %s\n", BN_bn2hex(message));
		
		rsaEXP(message , &client64Rsa);	//Decrypt with private keyExchange
		
		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}
	
	if(encType == RSA512){
		BIGNUM *message = BN_new();	
		message = BN_bin2bn((const unsigned char *) msg, length , NULL);
		
		printf("RSA512 : %s\n", BN_bn2hex(message));
		
		rsaEXP(message , &client512Rsa);	//Decrypt with private keyExchange

		BN_bn2bin(message , msg);

		BN_free(message);		
		return length;		
	}
	
}

int loadRsaKey(){
	
	server512Rsa.modulo 	 = BN_new();
	server512Rsa.exponent = BN_new();
	
	client512Rsa.modulo = BN_new();
	client512Rsa.exponent = BN_new();
	
	client64Rsa.modulo = BN_new();
	client64Rsa.exponent = BN_new();
	
	server64Rsa.modulo = BN_new();
	server64Rsa.exponent = BN_new();
	
	readRsaKey(ServerRsa512PublicKeyFile , NULL , &server512Rsa);
	
	readRsaKey(ClientRsa64PrivateKeyFile , clientName , &client64Rsa); 	
	
	readRsaKey(ServerRsa64PublicKeyFile , NULL , &server64Rsa);
	
	readRsaKey(ClientRsa512PrivateKeyFile , clientName , &client512Rsa); 	
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
	
	if(state == SHUTDOWN) //authentication error
	{
		fprintf(stderr , "\n **** key Exchange falied **** \n\n");
		fprintf(stderr , "\n **** closing connection **** \n\n");	
		exit(0);
	}
	
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
	
	fprintf(stderr , "\n< Client message , handshake > \n");			
	encrypt_and_send(ClientOpenConnection , strlen(ClientOpenConnection));
	fprintf(stderr, "%s" , ClientOpenConnection);

	fprintf(stderr , "\n< Server response , handshake > \n");			
	receive_and_decrypt(msg);

	if(strncmp(msg , ServerOpenConnection , strlen(ServerOpenConnection)) != 0)
	{
		fprintf(stderr, "handshake read error \n");
		return SHUTDOWN;
	}
	fprintf(stderr, "%s" , ServerOpenConnection);
		
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
	printf("%s\n", BN_bn2hex(message));
	
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
	
	fprintf(stderr , " **** Key Exhange phase **** \n\n");
	char cipher_spec[1];
	
	raedCipherSpec(ClientCipherSpecFile , clientName , cipher_spec);

	encType = PLAIN;
	
	fprintf(stderr , "\n< Client cipher spec , client key exchange > \n %c \n", cipher_spec[0]);		
	encrypt_and_send((byte *) cipher_spec , 1);	
	
	switch(cipher_spec[0]){
		
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
	
	fprintf(stderr , "\nCipher spec < %d , %d , %d > \n", cipherSpec.symmetric_cipher , cipherSpec.hash_function,  cipherSpec.public_cipher);	
	
	encType = cipherSpec.public_cipher; //use the rsa decided by the cipher spec, same for the clinet
	
	byte key24[3] , key64[8];
	BIGNUM *tm;
	tm = BN_new();
	int i;
	
	fprintf(stderr , "\n < Symmetric key , keyExchange >\n");
	
	if(cipherSpec.symmetric_cipher == Cipher_Bunny24)
	{
		receive_and_decrypt(key24);
		tm = BN_bin2bn((const unsigned char *) key24, 3 , NULL);
		
		bit buffer24[24];
		for(i = 0; i < 24; i++)
			buffer24[i] = (key24[i / 8] & (1 << i % 8)) ? 1 : 0;		//to bit
			
		cipherInit(&cipherStruct , buffer24 , 24 , init_vector , 24, Cipher_Bunny24);
	}
	else
	{
		receive_and_decrypt(key64);
		tm = BN_bin2bn((const unsigned char *) key64, 8 , NULL);
		
		bit buffer64[64];
		for(i = 0; i < 64; i++)
			buffer64[i] = (key64[i / 8] & (1 << i % 8)) ? 1 : 0;		//to bitLengthCrypt
		
		cipherInit(&cipherStruct , buffer64, 64 , init_vector , 24, cipherSpec.symmetric_cipher);
	}
	
	printf("PLAIN : %s\n", BN_bn2hex(tm));
	
	BN_free(tm);

	fprintf(stderr , "\n **** Key Exhange completed **** \n\n");
	
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
	
	encType = cipherSpec.symmetric_cipher;
	
	fprintf(stderr , "\n< Client message , communication > \n");			
	encrypt_and_send(msg , msgLength);	

	encType = PLAIN;
	
	fprintf(stderr , "\n< Server response , communication > \n");			
	msgLength = receive_and_decrypt(msg);
	printMsg("" ,msg ,(u_int16_t) msgLength);
	
	if(strncmp(msg , ServerCloseConnection , strlen(ServerCloseConnection)) == 0)	
		return SHUTDOWN;

	}while(1);
	
	return SHUTDOWN;
	
}

void closeConnection(){
	
	char msg[MSG_MAX_SIZE];
	
	encType = PLAIN;
	
	fprintf(stderr,"\n **** Closing phase **** \n\n");
	fprintf(stderr , "\n< Client message , closing > \n %s" , ClientCloseConnection);			
	encrypt_and_send(ClientCloseConnection , strlen(ClientCloseConnection));	
	
	fprintf(stderr , "\n< Server response , closing > \n");			
	receive_and_decrypt(msg);
	
	if(strncmp(msg , ServerCloseConnection , strlen(ServerCloseConnection)) != 0)
	{
		fprintf(stderr,"Closing read error \n\n");
		return;
	}
	else
		fprintf(stderr , "%s" , ServerCloseConnection);
	
	fprintf(stderr,"\n **** Closing succeded **** \n\n");

	closeChannel(inputChannel);
	closeChannel(outputChannel);
	clearRsaKey();
	cipherDelete(&cipherStruct, cipherSpec.symmetric_cipher);
	
}
