#ifndef _UTILITY_H
#define _UTILITY_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef u_int8_t bit;

void arraySum(bit *pA , bit *pB , bit *result, unsigned int degree);
void arrayMul(bit *pA , bit *pB , bit *field, bit *result, unsigned int degree); //degree is the length of the array == degree of the polynomial
void rotate(bit * array, int shift , unsigned int degree);
void printArray(bit *array, unsigned int x);
void inverteArray(bit *array, unsigned int length);
void arrayCopy(bit * a , bit * b , int length);

void hexToBinary(bit * array, u_int64_t hex, unsigned int length);


#endif
