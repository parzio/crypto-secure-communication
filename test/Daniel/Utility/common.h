#ifndef _COMMON_H_
#define _COMMON_H_

#include "../StreamChiper/streamChiper.h"
#include "../BlockChiper/cbcMode.h"
#include "../BlockChiper/SpongeBunny.h"
#include "../PRNG/PRNG.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/bn.h>

#define MSG_MAX_SIZE	2048	//bits
#define MAX_HEX_KEY_SIZE 256	//char

#define RSA512_BIT_LENGTH			512	//bits
#define RSA512_BYTE_LENGTH			RSA512_BIT_LENGTH / 8
#define RSA64_BIT_LENGTH			64
#define RSA64_BYTE_LENGTH 			RSA64_BIT_LENGTH / 8

#define ServerOpenConnection 	"HI!"
#define ClientOpenConnection 	"Hello!"

#define ClientCloseConnection "Bye"
#define ServerCloseConnection "ByeClient!"

#define totalUser			2

static bit init_vector[24] = {0 , 1 , 0 , 1 , 0 , 1 , 0 , 1, 0 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 1, 0 , 1 , 0 , 1};

static int length_of_bunny_message = 0;

typedef struct RSAKEY {
	BIGNUM *exponent;
	BIGNUM *modulo;
} rsaKey;

typedef enum { Cipher_Bunny24 = 1 , Cipher_MAJ5 = 3, Cipher_ALL5 = 2, hash_spongeBunny = 4 , RSA64 = 5 , RSA512 = 6 , PLAIN = 7} EncType;

typedef struct C_SPEC{
	EncType symmetric_cipher;
	EncType public_cipher;
	EncType hash_function;
} cipher_spec;

typedef struct cc_ss {	
	ALL5 all5;
	MAJ5 maj5;		
	bit * key;
	bit * vector;
	unsigned int vectorLength;
	unsigned int keyLength;
} cipher_struct;

int readFromPipe(const int in , byte *msg);
int writeInPipe(const int out, const byte *msg, u_int16_t size);

int 	readFromFile(FILE *file, char *string);

void readRsaKey(const char * filePath , const char * unserName , rsaKey  * key); //userName used by server
void raedCipherSpec(const char * filePath, const char * userName , char * cipherSpec);
int isInCipherSpec(const char * filePath, const char * cipherSpec);

void rsaEXP(BIGNUM *message , rsaKey * key);	//exponent

void 	printMsg(char *header , char *string , u_int16_t size);

int	openChannel(const char *path);
void 	closeChannel(int channel);

void cipherInit(cipher_struct * cipher , const bit * key , 
					 const int keyLength , const bit * initVector , const int vectorLength,
					 const EncType encType);

void cipherDelete(cipher_struct * cipher ,const EncType encType);


#endif
