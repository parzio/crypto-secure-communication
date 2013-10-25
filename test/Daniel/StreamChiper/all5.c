
#include "all5.h"

void ALL5_init(ALL5 *all5){
	
	LFSR temp = { NULL , NULL , 0};
	
	all5->reg[0] = temp;
	LFSR_init(&all5->reg[0] , ALL5_reg1 , ALL5_poly1 , ALL5_degree[0]);
	
	all5->reg[1] = temp;
	LFSR_init(&all5->reg[1] , ALL5_reg2 , ALL5_poly2 , ALL5_degree[1]);
	
	all5->reg[2] = temp;
	LFSR_init(&all5->reg[2] , ALL5_reg3 , ALL5_poly3 , ALL5_degree[2]);
	
	all5->reg[3] = temp;
	LFSR_init(&all5->reg[3] , ALL5_reg4 , ALL5_poly4 , ALL5_degree[3]);
	
	all5->reg[4] = temp;
	LFSR_init(&all5->reg[4] , ALL5_reg5 , ALL5_poly5 , ALL5_degree[4]);	
}

void ALL5_keyLoading(ALL5 *all5, bit key[] , bit vector[]){
	
	int i;
	
	for(i = 0; i < ALL5_keyDegree; i++)
	{
		int j;
		for(j = 0; j < 5; j++)
		{
			LFSR_update(&all5->reg[j]);
			all5->reg[j].reg[0] ^= key[i];		
		}		
	}
	
	for(i = 0; i < ALL5_frameVectorDegree; i++)
	{
		int j;
		for(j = 0; j < 5; j++)
		{
			LFSR_update(&all5->reg[j]);
			all5->reg[j].reg[0] ^= vector[i];		
		}		
	}
	
}

void ALL5_warmUpStream(ALL5 *all5, unsigned int stream){

	int i;
	for(i = 0; i < stream; i++)
		ALL5_update(all5);
	
}

void ALL5_delete(ALL5 *all5){	
	int i;	
	for(i = 0; i < 5; i++)
		LFSR_delete(&all5->reg[i]);
}

bit  ALL5_update(ALL5 *all5){
	
	//ALL5_printStatus(all5);
	
	int count_zero = 0;
	int i;
	bit out[5];
	
	for(i = 0; i < 5; i++)
		out[i] = LFSR_update(&all5->reg[i]);

	// Output -> (x1,x2,x3,x4,x5) -> x1*x4 + x2*x3 + x2*x5 + x3*x4

	bit result = (out[0] & out[3]) ^ (out[1] & out[2]) ^ (out[1] & out[4]) ^ (out[2] & out[3]);
		
	return result;
		
}

void ALL5_getStream(ALL5 *all5, bit *result, unsigned int stream){
	
	int i;
	
	for(i = 0; i < stream; i++)
		result[i] = ALL5_update(all5);	
	
}

void ALL5_printStatus(ALL5 *all5){
	
	printf("------------------------------------\n");
	int i;
	for(i = 0; i < 5; i++){
		LFSR_print(&all5->reg[i]);
	}
	printf("\n");	
}
