#ifndef _MYLIB_H
#define _MYLIB_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef u_int8_t bit;

typedef struct {	
	bit *reg;	//the register
	bit *poly;	//the polynomial
	unsigned int degree;	//the maximum degree of the polynomial
} LFSR;

void LFSR_init(LFSR *lfsr , bit reg[], bit poly[], unsigned int degree);
void LFSR_delete(LFSR *lfsr);
void LFSR_print(LFSR *lfsr);
bit  LFSR_update(LFSR *lfsr);
bit  LFSR_getBit(LFSR *lfsr, unsigned int position);
void LFSR_getStream(LFSR *lfsr , bit *result, unsigned int stream);


// 								0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24

/** MAJ5 init variables */

//TODO for the polynomial the order is 1 + x^0 bla bla, for the register the order doesn't matter

static bit MAJ5_reg1[] =  {0 , 1 , 0 , 0 , 1 , 0 , 0 , 1 , 0 , 1 , 1 , 0 , 1 , 1 , 1 , 0 , 0 , 0 , 0};
static bit MAJ5_poly1[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 1 , 1};
static bit MAJ5_reg2[] =  {1 , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 0 , 1 , 0};
static bit MAJ5_poly2[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1};
static bit MAJ5_reg3[] =  {1 , 1 , 1 , 0 , 1 , 0 , 0 , 1 , 1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 1 , 1};
static bit MAJ5_poly3[] = {1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1};
static bit MAJ5_reg4[] =  {1 , 0 , 0 , 0 , 1 , 1 , 0 , 1 , 1 , 1 , 0};
static bit MAJ5_poly4[] = {1 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1};
static bit MAJ5_reg5[] =  {1 , 1 , 1 , 1 , 0 , 1 , 0 , 0 , 1 , 1 , 1 , 1 , 1};
static bit MAJ5_poly5[] = {1 , 1 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1};

static bit MAJ5_frameVector[] = {0,0,1,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
static unsigned int MAJ5_frameVectorDegree = 22;

/** MAJ5 update variables */

static unsigned int MAJ5_degree[] = {19, 22, 23, 11, 13};
static unsigned int MAJ5_clock[]  = {9 - 1 , 11 - 1 , 11 - 1 , 5 - 1 , 7 - 1 };
/** prototype for maj5 */

typedef struct {
	LFSR reg[5];
	//TODO not all (only to show)
} MAJ5;

void MAJ5_init(MAJ5 *maj5);
void MAJ5_delete(MAJ5 *maj5);
bit  MAJ5_update(MAJ5 *maj5);
void MAJ5_printStatus(MAJ5 *maj5);
void MAJ5_getStream(MAJ5 *maj5, bit *result, unsigned int stream);
void MAJ5_encrypt(MAJ5 *maj5, bit *plaintext, bit *cypertext, unsigned int length);
void MAJ5_decrypt(MAJ5 *maj5, bit *plaintext, bit *cypertext, unsigned int length);

/** TODO notation

we'll use array of u_int8_t (alias bit) as the base type (at least for now)

now the order : 

an array {1 , 1 , 0 , 0 , 0 , 1} means 2^0 + 2^1 + 2^5 , or x^0 + x^1 + x^5.

so the order is natural (It's easier in this way (index , xor , bla bla) the only thing is that if we have
to do a left rotation we have to move the bits to the right and viceversa).
 

*/

void rotate(bit * array, int shift , unsigned int degree);
void printArray(bit *array, unsigned int x);


#endif
