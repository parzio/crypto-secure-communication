#include "mylib.h"

/** utility */

void printArray(bit *array, unsigned int x){	
	int i;
	for(i=0; i < x; i++)
		printf("%d " , array[i]);
	printf("\n");
}

void rotate(bit * array, int shift , unsigned int degree){
	
	// negative shift means left shift , positive means right shift
	
	//TODO shift remaninder
		
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

/** LFSR */

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

	/*printf("\tpoly : ");
	
	for(i = lfsr->degree ; i >= 0; i--)
		printf("%d " , lfsr->poly[i]);*/
	
	printf("\n");
	
}

bit  LFSR_getBit(LFSR *lfsr, unsigned int position){
		
		if(position < 0 || position >= lfsr->degree)
			return lfsr->reg[0];
	
		return lfsr->reg[position];
}

bit LFSR_update(LFSR *lfsr){
	
	unsigned int i;

	bit xored = 0;
	
	for(i = 1; i < (lfsr->degree + 1); i++)
	{
		if(lfsr->poly[i] == 1)
			xored ^= lfsr->reg[i - 1];
	}
	
	rotate(lfsr->reg , -1 , lfsr->degree);
	
	lfsr->reg[0] = xored;
	
	return xored; //output
					
}

void LFSR_getStream(LFSR *lfsr , bit *result, unsigned int stream){

	int i;
	for(i=0; i < stream; i++)
		result[i] = LFSR_update(lfsr);
		
}

/** LFSR end */
/** MAJ5 */

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

void MAJ5_delete(MAJ5 *maj5){	
	int i;	
	for(i = 0; i < 5; i++)
		LFSR_delete(&maj5->reg[i]);
}

bit  MAJ5_update(MAJ5 *maj5){
	
	MAJ5_printStatus(maj5);
	
	int count_zero = 0;
	int i;
	bit output = 0;
	bit temp = 0;
	
	for(i = 0; i < 5; i++)
	{
		temp = LFSR_getBit(&maj5->reg[i] , MAJ5_clock[i]);
		output ^= temp;
		if(temp == 0)
			count_zero ++;		
	
	}

	if(count_zero >= 3) //update all the 0 registers
	{
		for(i = 0; i < 5; i++)
			if(LFSR_getBit(&maj5->reg[i], MAJ5_clock[i]) == 0)
				LFSR_update(&maj5->reg[i]);
	}
	else	//update all the 1 registers
	{
		for(i = 0; i < 5; i++)
			if(LFSR_getBit(&maj5->reg[i], MAJ5_clock[i]) == 1)
				LFSR_update(&maj5->reg[i]);		
	}

	//TODO wrong output
	
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



