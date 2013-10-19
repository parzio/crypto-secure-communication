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

#define ServerOpenConnection 	"HI!"
#define ClientOpenConnection 	"Hello!"

#define ClinetCloseConnection "Bye"
#define ServerCloseConnection "ByeClient!"

#define Shutdown					"Goodbye"

int 	readFromPipe(const int in , u_int8_t **msg);
int 	writeInPipe(const int out, const u_int8_t **msg);
int	openChannel(const char *path);
void 	closeChannel(int channel);
//void  printMsg(FILE *file, const u_int8_t **msg);

#endif
