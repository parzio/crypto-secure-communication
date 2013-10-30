#ifndef _STREAMCHIPER_H
#define _STREAMCHIPER_H

#include "all5.h"
#include "maj5.h"
#include "a51.h"

typedef enum { Stream_MAJ5 , Stream_ALL5 , Stream_A51 } streamChiperSuite;

typedef struct {
	A51 	a51;
	ALL5 	all5;
	MAJ5	maj5;
	streamChiperSuite type;
} streamChiper;

void initStreamChiper(streamChiper *chiper , streamChiperSuite type , bit *key , bit *initVector);
void deleteStreamChiper(streamChiper *chiper );
void encrypt(streamChiper *chiper , byte *plaintext, byte *chipertext, int length);
void decrypt(streamChiper *chiper , byte *plaintext, byte *chipertext, int length);

#endif
