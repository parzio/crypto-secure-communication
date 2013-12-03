#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include <openssl/bio.h>
#include <openssl/bn.h>

#define MSG_MAX_SIZE	2048
#define MAX_HEX_KEY_SIZE 256


#define ServerOpenConnection 	"HI!"
#define ClientOpenConnection 	"Hello!"

#define ClientCloseConnection "Bye"
#define ServerCloseConnection "ByeClient!"

typedef u_int8_t byte;

enum chiperSuite { MAJ5 , ALL5 };

int 	readFromPipe(const int in , byte *msg);
int 	writeInPipe(const int out, const byte *msg, u_int16_t size);

int 	readFromFile(FILE *file, char *string);

void 	printMsg(char *header , char *string , u_int16_t size);

int	openChannel(const char *path);
void 	closeChannel(int channel);

#endif
