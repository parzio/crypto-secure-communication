#ifndef _COMMON_H_
#define _COMMON_H_

#include "../StreamChiper/streamChiper.h"
#include "../BlockChiper/cbcMode.h"
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


typedef struct RSAKEY {
	BIGNUM *exponent;
	BIGNUM *modulo;
} rsaKey;

typedef enum chiper_type {
	RSA64,
	RSA512,
	BUNNY24CBC,
	MAJ5,
	ALL5,
	PLAIN
} EncType;

int 	readFromPipe(const int in , byte *msg);
int 	writeInPipe(const int out, const byte *msg, u_int16_t size);

int 	readFromFile(FILE *file, char *string);

void readRsaKey(const char * filePath , const char * unserName , rsaKey  * key); //userName used by server

void rsaEXP(BIGNUM *message , rsaKey * key);	//exponent

void 	printMsg(char *header , char *string , u_int16_t size);

int	openChannel(const char *path);
void 	closeChannel(int channel);

#endif
