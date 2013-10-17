#ifndef _MYLIB_H
#define _MYLIB_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

inline u_int16_t BIT(u_int8_t shift);
inline u_int8_t sum(u_int8_t pA, u_int8_t pB);
inline u_int8_t toField(u_int8_t , u_int8_t degree);

u_int8_t product(u_int16_t field, u_int8_t pA, u_int8_t pB, u_int8_t degree);
u_int8_t rotate (u_int8_t pA , int shift , u_int8_t degree);

void print(u_int16_t);

#endif
