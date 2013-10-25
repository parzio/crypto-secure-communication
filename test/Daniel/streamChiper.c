#include "streamChiper.h"

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

/** ALL5 */

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

