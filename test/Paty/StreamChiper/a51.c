
#include "a51.h"

/** A51 */

void A51_init(A51 *a51){
	
	LFSR temp = { NULL , NULL , 0};
	
	a51->reg[0] = temp;
	LFSR_init(&a51->reg[0] , A51_reg1 , A51_poly1 , A51_degree[0]);
	
	a51->reg[1] = temp;
	LFSR_init(&a51->reg[1] , A51_reg2 , A51_poly2 , A51_degree[1]);
	
	a51->reg[2] = temp;
	LFSR_init(&a51->reg[2] , A51_reg3 , A51_poly3 , A51_degree[2]);

}

void A51_keyLoading(A51 *a51, bit key[] , bit vector[]){
	
	int i;
	
	for(i = 0; i < A51_keyDegree; i++)
	{
		int j;
		for(j = 0; j < 3; j++)
		{
			LFSR_update(&a51->reg[j]);
			a51->reg[j].reg[0] ^= key[i];		
		}		
	}
	
	for(i = 0; i < A51_frameVectorDegree; i++)
	{
		int j;
		for(j = 0; j < 3; j++)
		{
			LFSR_update(&a51->reg[j]);
			a51->reg[j].reg[0] ^= vector[i];		
		}		
	}
	
}

void A51_warmUpStream(A51 *a51, unsigned int stream){

	int i;
	for(i = 0; i < stream; i++)
		A51_update(a51);
	
}

void A51_delete(A51 *a51){	
	int i;	
	for(i = 0; i < 3; i++)
		LFSR_delete(&a51->reg[i]);
}

bit  A51_update(A51 *a51){

	int count_zero = 0;
	int i;
	bit output = 0;
	bit temp = 0;
	
	for(i = 0; i < 3; i++)
	{
		temp = LFSR_getBit(&a51->reg[i] , A51_clock[i]);
		if(temp == 0)
			count_zero ++;			
	}

	if(count_zero >= 2) //update all the 0 registers
	{
		for(i = 0; i < 3; i++)
		{
			if(LFSR_getBit(&a51->reg[i], A51_clock[i]) == 0)
				output ^= LFSR_update(&a51->reg[i]);
			else
				output ^= LFSR_getBit(&a51->reg[i], (a51->reg[i].degree-1));
		}
	}
	else	//update all the 1 registers
	{
		for(i = 0; i < 3; i++)
		{
			if(LFSR_getBit(&a51->reg[i], A51_clock[i]) == 1)
				output ^= LFSR_update(&a51->reg[i]);
			else
				output ^= LFSR_getBit(&a51->reg[i], (a51->reg[i].degree-1));
		}	
	}

	return output;
		
}

void A51_getStream(A51 *a51, bit *result, unsigned int stream){
	
	int i;
	
	for(i = 0; i < stream; i++)
		result[i] = A51_update(a51);	
	
}

void A51_printStatus(A51 *a51){
	
	printf("------------------------------------\n");
	int i;
	for(i = 0; i < 3; i++){
		LFSR_print(&a51->reg[i]);
	}
	printf("\n");	
}

void A51_encrypt(A51 *a51, byte *plaintext, byte *chipertext, unsigned int length){
		
	int i , j;
	
	for(i = 0; i < length; i++) // for each byte
	{
		bit temp[8];
		A51_getStream(a51 , temp , 8);
		
		byte currentByte = plaintext[i];
		
		for(j = 0; j < 8; j++)
		{
		bit x = temp[j] << j;	
		currentByte ^= x;			
		}
		
		chipertext[i] = currentByte;
		
	}
		
}


void A51_decrypt(A51 *a51, byte *plaintext, byte *chipertext, unsigned int length){
		
	int i , j;
	
	for(i = 0; i < length; i++) // for each byte
	{
		bit temp[8];
		A51_getStream(a51 , temp , 8);
		
		byte currentByte = chipertext[i];
		
		for(j = 0; j < 8; j++)
		{
		bit x = temp[j] << j;	
		currentByte ^= x;			
		}
		
		plaintext[i] = currentByte;
		
	}
		
}