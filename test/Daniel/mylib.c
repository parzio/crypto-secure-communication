#include "mylib.h"


inline u_int8_t BIT(u_int8_t shift){
	return 1 << shift;
}

inline u_int8_t sum(u_int8_t pA, u_int8_t pB){
	return (pA ^ pB);		
}

inline u_int8_t toField(u_int8_t pA ,u_int8_t degree){ // make the bit out of the field 0	
	int i;
	for(i = 7 ; i >= degree ; i--) 
		if(pA & BIT(i))
			pA ^= BIT(i);
	return pA;
}

u_int8_t product(u_int16_t field, u_int8_t pA, u_int8_t pB, u_int8_t degree){
	
	pB = toField(pB , degree);	
	u_int16_t result = 0;	
	int i=0;
	for(i=0; i < degree; i++) 
	{
		if(pA & BIT(i)) 	//check if bit i of pA is 1
			result ^= pB; 	//sum in field 2 is XOR so x^2 + x^2 = 0 (I think). 
		pB <<=1;
	}
	
	//TODO remainder
	
	return (u_int8_t) result;
	
}

u_int8_t rotate (u_int8_t pA , int shift , u_int8_t degree){

	degree --;
	int i = 0;
	u_int8_t mask = 0;
	if(shift <= 0)	//left rotation
	{
		for(i = 0; i < abs(shift); i++)
		{
			mask = (pA & BIT(degree)) ? 1 : 0; //check the first bit 
			pA <<= 1;	//left shift
			pA |= mask;	//add the removed bit
		}
	}
	else
	{
		for(i = 0; i < (unsigned) shift; i++)
		{
			mask = (pA & BIT(0)) ? 1 : 0;	//check the first bit
			pA >>= 1;	//right shift
			if(mask == 1)
				pA |= BIT(degree); //add the removed bit 
		}
	}

	return pA;
}

void print(u_int8_t a){
	
	int l = sizeof(a) * 8;	
	unsigned char bits[l];
	printf("num : %d -> \t", a);
	int i;
	for ( i = 0; i < l; i++) {
		bits[i] = a & 1;
		a >>= 1;
	}
	for ( i = l-1; i >= 0; i--) {
		printf("%d ",bits[i]);
	}
	printf("\n");
}



