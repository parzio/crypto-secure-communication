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
void LFSR_getStream(LFSR *lfsr , bit *result, unsigned int stream);


//static bit MAJ5_reg1[] = {1};
//static bit MAJ5_poly1[] = {1};
//static bit MAJ5_reg2[] = {1};
//static bit MAJ5_poly2[] = {1};
//static bit MAJ5_reg3[] = {1};
//static bit MAJ5_poly3[] = {1};
//static bit MAJ5_reg4[] = {1};
//static bit MAJ5_poly4[] = {1};
//static bit MAJ5_reg5[] = {1};
//static bit MAJ5_poly5[] = {1};
//static unsigned int MAJ5_degree1 = {1};
//static unsigned int MAJ5_degree2 = {1};
//static unsigned int MAJ5_degree3 = {1};
//static unsigned int MAJ5_degree4 = {1};
//static unsigned int MAJ5_degree5 = {1};

/** prototype for maj5 */

typedef struct {
	LFSR reg1, reg2 , reg3 , reg4 , reg5;
	unsigned int clockReg1 , clockReg2, clockReg3, clockReg4, clockReg5;
	//TODO not all (only to show)
} MAJ5;

void MAJ5_init();
void MAJ5_delete();
void MAJ5_update();
void MAJ5_getStream();
//TODO bla bla bla

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
