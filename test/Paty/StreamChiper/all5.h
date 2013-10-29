#ifndef _ALL5_H
#define _ALL5_H

#include "lfsr.h"

/** ALL5 init variables , same as MAJ5 */

static bit ALL5_reg1[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit ALL5_reg2[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit ALL5_reg3[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit ALL5_reg4[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
static bit ALL5_reg5[] =  {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};

static bit ALL5_poly1[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 1 , 1};
static bit ALL5_poly2[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1};
static bit ALL5_poly3[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1};
static bit ALL5_poly4[] = {1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1};
static bit ALL5_poly5[] = {1 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1};

static unsigned int ALL5_frameVectorDegree = 22;
static unsigned int ALL5_keyDegree = 64;

/** ALL5 update variables */

static unsigned int ALL5_degree[] = {19, 22, 23, 11, 13};

typedef struct {
	LFSR reg[5];
} ALL5;

void ALL5_init(ALL5 *all5);
void ALL5_delete(ALL5 *all5);
void ALL5_keyLoading(ALL5 *all5, bit key[] , bit vector[]);
void ALL5_warmUpStream(ALL5 *all5, unsigned int stream);
bit  ALL5_update(ALL5 *all5);
void ALL5_printStatus(ALL5 *all5);
void ALL5_getStream(ALL5 *all5, bit *result, unsigned int stream);
void ALL5_encrypt(ALL5 *all5, bit *plaintext, bit *chipertext, unsigned int length);
void ALL5_decrypt(ALL5 *all5, bit *plaintext, bit *chipertext, unsigned int length);

#endif
