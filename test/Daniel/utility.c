
#include "utility.h"

void printArray(bit *array, unsigned int x){	
	int i;
	for(i=0; i < x; i++)
		printf("%d " , array[i]);
	printf("\n");
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
