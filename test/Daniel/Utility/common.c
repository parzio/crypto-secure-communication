#include "common.h"

int openChannel(const char * path){
	int fd;
	/* Open the FIFO in pathname */
	if( (fd = open(path,O_RDWR)) < 0 ) {
		perror("open()");
		exit(1);
	}

	return (fd);
}

void closeChannel(int channel_fd){
	/* Close the file descriptor */
	close(channel_fd);
}

int readFromPipe(const int in , byte *msg){
	
	int read_len;
	u_int16_t msg_size;

	/* The first 16 bits represents the message size */
	if(read(in ,(void *)&msg_size , sizeof(msg_size)) < 0 ) {
		perror("readSize()");
		return -1;
	}

	/* Verify that the size of the message is no more than MSG_SIZE_MAX bytes */
	if( msg_size >= MSG_MAX_SIZE ) {
		perror("Message too large\n");
		return -1;
	}

	char buffer[MSG_MAX_SIZE];
	
	/* Read msg_size bytes from the channel */
	read_len = read(in , (void *)buffer , msg_size);

	if( read_len < 0 ) {
		perror("readMsg()");
		return -1;
	}
	
	memcpy(msg, buffer , read_len);
	
	return read_len;
		
}

int writeInPipe(const int out , const byte *msg , u_int16_t size){

	/* Send the size */
	if(write(out , (const void *)&size , sizeof(size)) < 0 ) {
		perror("writeSize()");
		return -1;
	}

	u_int16_t written = write(out ,(const void *)msg, size);
	
	/* Send the message */
	if( written  < 0 ) {
		perror("writeMsg()");
		return -1;
	}
	
	return written;
}

int readFromFile(FILE *file, char *string){
	
	int read;
	size_t length = MSG_MAX_SIZE;
	
	do{	
	read = getline(&string , &length, file);
	
	if(read == -1)
		return -1;
	else
		return read;
	
	}while((string[0] == '/') && (string[1] == '/'));
	
}

void rsaEXP(BIGNUM *message , rsaKey * key){

	BN_CTX * t = BN_CTX_new();

	BN_mod_exp(message , message, key->exponent , key->modulo , t);
	
	BN_CTX_free(t);

}

int isInCipherSpec(const char * filePath, const char * cipherSpec){
	FILE *file = fopen(filePath, "rt");

	if(file == NULL){
		perror(filePath);
		return -1;
	}
	
	char cipherss[20];
	int exit = 0;
	
	do{
		int read = readFromFile(file , cipherss);
		
		if(read <= 0)
			return - 1;

		if(cipherss[0] == cipherSpec[0])
			return 1;
		
	}while(exit == 0);
	
	fclose(file);	
	
}

void raedCipherSpec(const char * filePath, const char * userName , char * cipherSpec){
	
	FILE *file = fopen(filePath, "rt");

	if(file == NULL || userName == NULL){
		perror(filePath);
		return -1;
	}
	
	char name[512];
	char cipherss[20];
	int userChar;
	int exit = 0;
	
	do{
		userChar = readFromFile(file , name);
		readFromFile(file , cipherss);
		
		if(userChar <= 0)
			return;
		
		name[userChar] = '\0';

		if(strncmp(name , userName , strlen(userName)) == 0)
		{
		cipherSpec[0] = cipherss[0];
		exit = 1;			
		}
		
	}while(exit == 0);
	
	fclose(file);	
	
}

void readRsaKey(const char * filePath , const char * userName , rsaKey * key){
		
	FILE *file = fopen(filePath, "rt");

	if(file == NULL){
		perror(filePath);
		return -1;
	}
	
	char exponent_string[MAX_HEX_KEY_SIZE] , modulo_string[MAX_HEX_KEY_SIZE];
	char name[512];
	int userChar = 10 , exponent_length = 0 , modulo_length = 0;
	int exit = 0;
	do{
		if(userName != NULL)
			userChar = readFromFile(file , name);

		name[userChar] = '\0';
		
		exponent_length = readFromFile(file , exponent_string);		
		modulo_length = readFromFile(file , modulo_string);

		if(modulo_length <= 0 || exponent_length <= 0 || userChar <= 0)
			return;
		
		if(userName != NULL && strncmp(name , userName , strlen(userName)) == 0)
		{
		BN_hex2bn(&key->modulo, modulo_string);
		BN_hex2bn(&key->exponent, exponent_string);
		exit = 1;			
		}
		else	if(userName == NULL)
		{
		BN_hex2bn(&key->modulo, modulo_string);
		BN_hex2bn(&key->exponent, exponent_string);
		exit = 1;
		}
	}while(exit == 0);
	
	fclose(file);

}

void printMsg(char *header , char *string , u_int16_t size){	
	int i ;
	
	fprintf(stderr , "%s" , header);
	
	for(i = 0; i < size ; i++)
		if(string[i] != '\n')
			fprintf(stderr , "%c" , string[i]);
	
	fprintf(stderr , "\n");

}

void cipherInit(cipher_struct * cipher , const bit * key , const int keyLength , const bit * initVector , const int vectorLength, const EncType encType){
	cipher->keyLength 	= keyLength;
	cipher->vectorLength = vectorLength;
	
	cipher->key  = malloc(keyLength * sizeof(bit));	//register
	cipher->vector  = malloc(vectorLength * sizeof(bit));	//register
	
	memcpy(cipher->key , key , sizeof(bit) * keyLength);
	memcpy(cipher->vector , initVector , sizeof(bit) * vectorLength);
	
	if(encType == Cipher_MAJ5)
	{
		MAJ5_init(&cipher->maj5);
		MAJ5_keyLoading(&cipher->maj5 , key , initVector);
		MAJ5_warmUpStream(&cipher->maj5 , 100);
		return;
	}
	
	if(encType == Cipher_ALL5)
	{
		ALL5_init(&cipher->all5);
		ALL5_keyLoading(&cipher->all5 , key , initVector);
		ALL5_warmUpStream(&cipher->all5 , 100);	
		return;
	}
}

void cipherDelete(cipher_struct * cipher, const EncType encType){
	
	if(encType == Cipher_MAJ5)
		MAJ5_delete(&cipher->maj5);

	if(encType == Cipher_ALL5)
		ALL5_delete(&cipher->all5);

	if(cipher->key != NULL)
		free(cipher->key);
	
	if(cipher->vector != NULL)
		free(cipher->vector);
}

