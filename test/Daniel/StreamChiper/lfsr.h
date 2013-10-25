#ifndef _LFSR_H
#define _LFSR_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "utility.h"

typedef u_int8_t bit;
typedef u_int8_t byte;

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

#endif