#ifndef _TOY_H
#define _TOY_H

#include "lfsr.h"

// 								0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24

/** TOY init variables */

//TODO for the polynomial the order is 1 + x^0 bla bla, for the register the order doesn't matter

static bit TOY_poly1[] = {1 , 1 , 1};
static bit TOY_poly2[] = {1 , 1 , 0 , 1};
static bit TOY_poly3[] = {1 , 1 , 0 , 0 , 1};
static bit TOY_poly4[] = {1 , 1 , 0 , 0 , 0 , 0 , 0 , 1};
static bit TOY_poly5[] = {1 , 1 , 1 , 0 , 0 , 0 , 0 , 1 , 1};

/** TOY update variables */

static unsigned int TOY_degree[] = {2, 3, 4, 7, 8};
/** prototype for toy */

typedef struct {
	LFSR reg[5];
} TOY;

void TOY_init(TOY *toy);
void TOY_delete(TOY *toy);
bit  TOY_update(TOY *toy);
void TOY_printStatus(TOY *toy);
void TOY_getStream(TOY *toy, bit *result, unsigned int stream);
void TOY_print_intial_state(TOY * toy);
void TOY_init_with_reg(TOY * toy , bit r1[2] , bit r2[3] , bit r3[4] , bit r4[7] , bit r5[8]);

#endif
