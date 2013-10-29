#ifndef _A51_H
#define _A51_H

#include "lfsr.h"

/** A51 init variables */

//TODO for the polynomial the order is 1 + x^0 bla bla, for the register the order doesn't matter
// 							  0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24

static bit A51_reg1[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit A51_reg2[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit A51_reg3[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};

static bit A51_poly1[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 1 , 1};
static bit A51_poly2[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1};
static bit A51_poly3[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1};

static unsigned int A51_frameVectorDegree = 22;
static unsigned int A51_keyDegree = 64;

/** MAJ5 update variables */

static unsigned int A51_degree[] = {19, 22, 23, 11, 13};
static unsigned int A51_clock[]  = {9 - 1 , 11 - 1 , 11 - 1};
/** prototype for maj5 */

typedef struct {
	LFSR reg[3];
} A51;

void A51_init(A51 *a51);
void A51_delete(A51 *a51);
void A51_keyLoading(A51 *a51, bit key[] , bit vector[]);
void A51_warmUpStream(A51 *a51, unsigned int stream);
bit  A51_update(A51 *a51);
void A51_printStatus(A51 *a51);
void A51_getStream(A51 *a51, bit *result, unsigned int stream);
void A51_encrypt(A51 *a51, byte *plaintext, byte *chipertext, unsigned int length);
void A51_decrypt(A51 *a51, byte *plaintext, byte *chipertext, unsigned int length);




#endif
