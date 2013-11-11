
#include "utility.h"

void printArray(bit *array, unsigned int x){	
	int i;
	for(i=0; i < x; i++)
		printf("%d " , array[i]);
	printf("\n");
}

void arraySum(bit *pA , bit *pB , bit *result, unsigned int degree){	
	int i;	
	for(i = 0; i < degree; i++)
		result[i] = pA[i] ^ pB[i];	
}

void arrayMul(bit *pA , bit *pB , bit *field, bit *result, unsigned int degree){
	
	bit temp[degree * 2];
	int i;
	for(i = 0; i < degree * 2; i++)
		temp[i] = 0;
	
	
	for(i = 0; i < degree; i++)
	{
		if(pB[i] == 1)	//if the bit i of B is 1
		{
			int j;		//Sum pA shifted of i position
			int z = i;
			for(j=0; j < degree; j++)
			{
				temp[z] ^= pA[j]; 
				z++;
			}		
		}		
	}


	for(i = (degree * 2) - 1; i >= degree; i--)
	{
		if(temp[i] == 1)
		{
			int j = i;
			int z;
			for(z = degree; z >= 0; z--){  
				temp[j] ^= field[z];
				j--;				
			}
		}			
	}
	
	for(i = 0; i < degree; i++)
		result[i] = temp[i];
}

int arrayToByte(bit *bits, bit *bytes, int length){
	int i;
	for(i = 0; i < length; i++)
	{
		int j = i / 8; // byte n
		bytes[j] |= bits[i];		
	}		
	return length/8; //length of the returned array
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

void inverteArray(bit *array, unsigned int length){
	
	int i;
	bit buffer[length];
	
	length --;
	
	for(i = 0; i <= length; i++)
		buffer[length - i] = array[i];
	
	for(i = 0; i <= length; i++)
		array[i] = buffer[i];	
}

void hexToBinary(bit * array, u_int64_t hex, unsigned int length){
	
	int i;
	for(i = 0; i < length; i++)
	{
		if(hex & (1 << i))
			array[i] = 1;
		else
			array[i] = 0;
	}
}
/*
void BinaryToHex(bit * array, u_int64_t hex, unsigned int length){
	
	int i;
	for(i = 0; i < length; i++)
	{
		if(hex & (1 << i))
			array[i] = 1;
		else
			array[i] = 0;
	}
}

*/
void arrayCopy(bit * a , bit * b , int length){
	
	int i;
	
	for(i = 0; i < length; i++)
		b[i] = a[i];
	
}

