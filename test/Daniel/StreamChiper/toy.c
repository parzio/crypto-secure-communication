
#include "toy.h"
#include "../PRNG/PRNG.h"

int isZero(bit * reg , int length){
	int x = 0;
	int j = 0;
	for(x = 0; x < length; x++)
		if(reg[x] == 0)
			j++;
		
	if(j == length)
		return 0;
	else
		return 1;
}

void TOY_init(TOY *toy){
	
	LFSR temp = { NULL , NULL , 0};
	
	bit TOY_reg1[TOY_degree[0]];
	bit TOY_reg2[TOY_degree[1]];
	bit TOY_reg3[TOY_degree[2]];
	bit TOY_reg4[TOY_degree[3]];
	bit TOY_reg5[TOY_degree[4]];
	
	initFPRNG();
	int r1 = 0 , r2 = 0, r3 = 0 , r4 = 0, r5 = 0;
	do{	
	
	SPRNG(TOY_reg1 , TOY_degree[0]);
	r1 = isZero(TOY_reg1 , TOY_degree[0]);
	
	SPRNG(TOY_reg2 , TOY_degree[1]);
	r2 = isZero(TOY_reg2 , TOY_degree[1]);
	
	SPRNG(TOY_reg3 , TOY_degree[2]);
	r3 = isZero(TOY_reg3 , TOY_degree[2]);
	
	SPRNG(TOY_reg4 , TOY_degree[3]);
	r4 = isZero(TOY_reg4 , TOY_degree[3]);
	
	SPRNG(TOY_reg5 , TOY_degree[4]);
	r5 = isZero(TOY_reg5 , TOY_degree[4]);
	
	}while(r1 == 0 || r2 == 0 || r3 == 0 || r4 == 0 || r5 == 0);
	
	toy->reg[0] = temp;
	LFSR_init(&toy->reg[0] , TOY_reg1 , TOY_poly1 , TOY_degree[0]);
	
	toy->reg[1] = temp;
	LFSR_init(&toy->reg[1] , TOY_reg2 , TOY_poly2 , TOY_degree[1]);
	
	toy->reg[2] = temp;
	LFSR_init(&toy->reg[2] , TOY_reg3 , TOY_poly3 , TOY_degree[2]);
	
	toy->reg[3] = temp;
	LFSR_init(&toy->reg[3] , TOY_reg4 , TOY_poly4 , TOY_degree[3]);
	
	toy->reg[4] = temp;
	LFSR_init(&toy->reg[4] , TOY_reg5 , TOY_poly5 , TOY_degree[4]);	
}

void TOY_init_with_reg(TOY * toy , bit r1[2] , bit r2[3] , bit r3[4] , bit r4[7] , bit r5[8]){
	
	LFSR temp = { NULL , NULL , 0};
	
	toy->reg[0] = temp;
	LFSR_init(&toy->reg[0] , r1 , TOY_poly1 , TOY_degree[0]);
	
	toy->reg[1] = temp;
	LFSR_init(&toy->reg[1] , r2 , TOY_poly2 , TOY_degree[1]);
	
	toy->reg[2] = temp;
	LFSR_init(&toy->reg[2] , r3 , TOY_poly3 , TOY_degree[2]);
	
	toy->reg[3] = temp;
	LFSR_init(&toy->reg[3] , r4 , TOY_poly4 , TOY_degree[3]);
	
	toy->reg[4] = temp;
	LFSR_init(&toy->reg[4] , r5 , TOY_poly5 , TOY_degree[4]);
}

void TOY_print_intial_state(TOY * toy){
	printf("Reg initial state \n------------------------- \n");
	
	LFSR * reg1 , *reg2 , *reg3 , *reg4 , *reg5;
	
	reg1 = &toy->reg[0];
	reg2 = &toy->reg[1];
	reg3 = &toy->reg[2];
	reg4 = &toy->reg[3];
	reg5 = &toy->reg[4];
	
	printArray(reg1->reg,  TOY_degree[0]);
	printArray(reg2->reg , TOY_degree[1]);
	printArray(reg3->reg , TOY_degree[2]);
	printArray(reg4->reg , TOY_degree[3]);
	printArray(reg5->reg , TOY_degree[4]);
	
	printf("------------------------- \n");	
}

void TOY_delete(TOY *toy){	
	int i;	
	for(i = 0; i < 5; i++)
		LFSR_delete(&toy->reg[i]);
}

bit  TOY_update(TOY *toy){

	bit out[5];
	int i;
	for(i = 0; i < 5; i++)
		out[i] = LFSR_update(&toy->reg[i]);
	//x1*x2*x3*x4*x5 + x4*x5 + x5; 
	
	bit result = (out[0] & out[1] & out[2] & out[3] & out[4]) ^ (out[3] & out[4]) ^ out[4];
	
	return result;
		
}

void TOY_getStream(TOY *toy, bit *result, unsigned int stream){
	
	int i;
	
	for(i = 0; i < stream; i++)
		result[i] = TOY_update(toy);	
	
}

void TOY_printStatus(TOY *toy){
	
	printf("------------------------------------\n");
	int i;
	for(i = 0; i < 5; i++){
		LFSR_print(&toy->reg[i]);
	}
	printf("\n");	
}

