#ifndef _MAJ5_H
#define _MAJ5_H

#include "lfsr.h"

// 								0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24

/** MAJ5 init variables */

//TODO for the polynomial the order is 1 + x^0 bla bla, for the register the order doesn't matter

static bit MAJ5_reg1[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit MAJ5_reg2[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit MAJ5_reg3[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit MAJ5_reg4[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit MAJ5_reg5[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};

static bit MAJ5_poly1[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 1 , 1};
static bit MAJ5_poly2[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1};
static bit MAJ5_poly3[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1};
static bit MAJ5_poly4[] = {1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1};
static bit MAJ5_poly5[] = {1 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1};

static unsigned int MAJ5_frameVectorDegree = 22;
static unsigned int MAJ5_keyDegree = 64;

/** MAJ5 update variables */

static unsigned int MAJ5_degree[] = {19, 22, 23, 11, 13};
static unsigned int MAJ5_clock[]  = {9 - 1 , 11 - 1 , 11 - 1 , 5 - 1 , 7 - 1 };
/** prototype for maj5 */

typedef struct {
	LFSR reg[5];
} MAJ5;

void MAJ5_init(MAJ5 *maj5);
void MAJ5_delete(MAJ5 *maj5);
void MAJ5_keyLoading(MAJ5 *maj5, bit key[] , bit vector[]);
void MAJ5_warmUpStream(MAJ5 *maj5, unsigned int stream);
bit  MAJ5_update(MAJ5 *maj5);
void MAJ5_printStatus(MAJ5 *maj5);
void MAJ5_getStream(MAJ5 *maj5, bit *result, unsigned int stream);
void MAJ5_encrypt(MAJ5 *maj5, bit *plaintext, bit *cypertext, unsigned int length);
void MAJ5_decrypt(MAJ5 *maj5, bit *plaintext, bit *cypertext, unsigned int length);


#endif
