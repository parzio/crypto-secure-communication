
#include "maj5.h"

void MAJ5_init(MAJ5 *maj5){
	
	LFSR temp = { NULL , NULL , 0};
	
	maj5->reg[0] = temp;
	LFSR_init(&maj5->reg[0] , MAJ5_reg1 , MAJ5_poly1 , MAJ5_degree[0]);
	
	maj5->reg[1] = temp;
	LFSR_init(&maj5->reg[1] , MAJ5_reg2 , MAJ5_poly2 , MAJ5_degree[1]);
	
	maj5->reg[2] = temp;
	LFSR_init(&maj5->reg[2] , MAJ5_reg3 , MAJ5_poly3 , MAJ5_degree[2]);
	
	maj5->reg[3] = temp;
	LFSR_init(&maj5->reg[3] , MAJ5_reg4 , MAJ5_poly4 , MAJ5_degree[3]);
	
	maj5->reg[4] = temp;
	LFSR_init(&maj5->reg[4] , MAJ5_reg5 , MAJ5_poly5 , MAJ5_degree[4]);	
}

void MAJ5_keyLoading(MAJ5 *maj5, bit key[] , bit vector[]){
	
	int i;
	
	for(i = 0; i < MAJ5_keyDegree; i++)
	{
		int j;
		for(j = 0; j < 5; j++)
		{
			LFSR_update(&maj5->reg[j]);
			maj5->reg[j].reg[0] ^= key[i];		
		}		
	}
	
	for(i = 0; i < MAJ5_frameVectorDegree; i++)
	{
		int j;
		for(j = 0; j < 5; j++)
		{
			LFSR_update(&maj5->reg[j]);
			maj5->reg[j].reg[0] ^= vector[i];		
		}		
	}
	
}

void MAJ5_warmUpStream(MAJ5 *maj5, unsigned int stream){

	int i;
	for(i = 0; i < stream; i++)
		MAJ5_update(maj5);
	
}

void MAJ5_delete(MAJ5 *maj5){	
	int i;	
	for(i = 0; i < 5; i++)
		LFSR_delete(&maj5->reg[i]);
}

bit  MAJ5_update(MAJ5 *maj5){

	int count_zero = 0;
	int i;
	bit output = 0;
	bit temp = 0;
	
	for(i = 0; i < 5; i++)
	{
		temp = LFSR_getBit(&maj5->reg[i] , MAJ5_clock[i]);
		if(temp == 0)
			count_zero ++;			
	}

	if(count_zero >= 3) //update all the 0 registers
	{
		for(i = 0; i < 5; i++)
		{
			if(LFSR_getBit(&maj5->reg[i], MAJ5_clock[i]) == 0)
				output ^= LFSR_update(&maj5->reg[i]);
			else
				output ^= LFSR_getBit(&maj5->reg[i], (maj5->reg[i].degree-1));
		}
	}
	else	//update all the 1 registers
	{
		for(i = 0; i < 5; i++)
		{
			if(LFSR_getBit(&maj5->reg[i], MAJ5_clock[i]) == 1)
				output ^= LFSR_update(&maj5->reg[i]);
			else
				output ^= LFSR_getBit(&maj5->reg[i], (maj5->reg[i].degree-1));
		}	
	}

	return output;
		
}

void MAJ5_getStream(MAJ5 *maj5, bit *result, unsigned int stream){
	
	int i;
	
	for(i = 0; i < stream; i++)
		result[i] = MAJ5_update(maj5);	
	
}

void MAJ5_printStatus(MAJ5 *maj5){
	
	printf("------------------------------------\n");
	int i;
	for(i = 0; i < 5; i++){
		LFSR_print(&maj5->reg[i]);
	}
	printf("\n");	
}