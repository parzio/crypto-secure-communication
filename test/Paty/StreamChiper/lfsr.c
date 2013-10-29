
#include "lfsr.h"

void LFSR_init(LFSR *lfsr , bit reg[], bit poly[], unsigned int degree){
	
	lfsr->reg  = malloc(degree * sizeof(bit));	//register
	lfsr->poly = malloc((degree + 1) * sizeof(bit));	//polynomial
	lfsr->degree = degree;	//degree of the polynomial
	
	int i;
	
	for(i=0 ; i < degree; i++){
		lfsr->reg[i]  = reg[i];	
		lfsr->poly[i] = poly[i];
	}
	
	lfsr->poly[degree] = poly[degree];
		
}

void LFSR_delete(LFSR *lfsr){	
	
	if(lfsr->reg != NULL)
		free(lfsr->reg);
	
	if(lfsr->poly != NULL)
		free(lfsr->poly);
}

void LFSR_print(LFSR *lfsr){	
	
	if(lfsr->reg == NULL || lfsr->poly == NULL || lfsr->degree == 0)
	{
		printf("reg empty!!!");
		return;		
	}

	int i = 0;
	
	printf("LFSR reg : ");
	
	for(i = 0 ; i < lfsr->degree ; i++)
		printf("%d " , lfsr->reg[i]);

	printf("\n");
	
}

bit  LFSR_getBit(LFSR *lfsr, unsigned int position){
		
		if(position < 0 || position >= lfsr->degree)
		{
			perror("index of out range");
			return lfsr->reg[0];
		}
		return lfsr->reg[position];
}

bit LFSR_update(LFSR *lfsr){
	
	unsigned int i;

	bit xored = 0;
	bit output = lfsr->reg[lfsr->degree - 1];
	
	for(i = 1; i < (lfsr->degree + 1); i++)
	{
		if(lfsr->poly[i] == 1)
			xored ^= lfsr->reg[i - 1];
	}
	
	rotate(lfsr->reg , -1 , lfsr->degree);
	
	lfsr->reg[0] = xored;
	
	//the output is the bit shiftet to the right
	
	return output; //output
					
}

void LFSR_getStream(LFSR *lfsr , bit *result, unsigned int stream){

	int i;
	for(i=0; i < stream; i++)
		result[i] = LFSR_update(lfsr);
		
}
