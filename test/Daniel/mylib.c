#include "mylib.h"

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
	
	for(i = lfsr->degree-1 ; i >= 0; i--)
		printf("%d " , lfsr->reg[i]);

	printf("\tpoly : ");
	
	for(i = lfsr->degree ; i >= 0; i--)
		printf("%d " , lfsr->poly[i]);
	
	printf("\n");
	
}

bit LFSR_update(LFSR *lfsr){
	
	unsigned int i;

	bit xored = 0;
	
	for(i = 1; i < (lfsr->degree + 1); i++)
	{
		if(lfsr->poly[i] == 1)
			xored ^= lfsr->reg[i - 1]; //TODO not sure	
	}
	
	rotate(lfsr->reg , 1 , lfsr->degree);
	
	lfsr->reg[lfsr->degree - 1] = xored;
	
	return xored; //output
					
}

void LFSR_getStream(LFSR *lfsr , bit *result, unsigned int stream){

	int i;
	for(i=0; i < stream; i++)
		result[i] = LFSR_update(lfsr);
		
}

void printArray(bit *array, unsigned int x){	
	int i;
	for(i=0; i < x; i++)
		printf("%d " , array[i]);
	printf("\n");
}

void rotate(bit * array, int shift , unsigned int degree){
	
	// negative shift means left shift , positive means right shift
	
	bit buffer[degree];
	
	int i;
	
	if(shift < 0) //left , (means right)
	{
		int j = 0;
		for(i = (degree - abs(shift)) ; i < degree; i++) 
		{
			buffer[j] = array[i];
			j++;
		}
		for(i = 0; i < (degree - abs(shift)) ; i++)
		{
			buffer[j] = array[i];
			j++;	
		}	
	}
	else	//right, (means left)
	{
		int j = 0;
		for(i = shift ; i < degree ; i++)
		{
			buffer[j] = array[i];
			j++;	
		}		
		for(i = 0 ; i < shift ; i++)
		{
			buffer[j] = array[i];
			j++;	
		}		
	}
	
	for(i=0; i < degree; i++)
		array[i] = buffer[i];	
}






