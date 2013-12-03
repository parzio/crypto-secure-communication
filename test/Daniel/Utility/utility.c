
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

void hexToBinary(bit * array, char * hex){

	/*
	 * I've changed the hex to bin --> now 0x03 --> [1,1,0,0,0,0,0,0] ---> correct 
	 */
	
	int i, j , index = 0;
		
	for(i = strlen(hex) - 1; i >= 0; i--)
	{
		u_int8_t num = 0;
		
		switch(hex[i]){
			case '0' : num = 0;  break;
			case '1' : num = 1;  break;
			case '2' : num = 2;  break;
			case '3' : num = 3;  break;	
			case '4' : num = 4;  break;
			case '5' : num = 5;  break;	
			case '6' : num = 6;  break;
			case '7' : num = 7;  break;	
			case '8' : num = 8;  break;
			case '9' : num = 9;  break;
			case 'A' : num = 10; break;	
			case 'B' : num = 11; break;
			case 'C' : num = 12; break;	
			case 'D' : num = 13; break;
			case 'E' : num = 14; break;
			case 'F' : num = 15; break;
			case 'a' : num = 10; break;	
			case 'b' : num = 11; break;
			case 'c' : num = 12; break;	
			case 'd' : num = 13; break;
			case 'e' : num = 14; break;
			case 'f' : num = 15; break;
			default : printf("error hex for char %c \n\n" , hex[i]); break;
 		}

		for(j = 0; j < 4; j++)
		{
			if(num & (1 << j))
				array[index] = 1;
			else
				array[index] = 0;
			index++;
		}
	}
}

void printHex(bit * array, unsigned int length){
	
	int i,j;
	int num_of_hex = length / 4 + ((length % 4 != 0) ? 1 : 0);	//number of hex that I have to print	
	unsigned char buffer[num_of_hex];	//buffer
	memset(buffer , 0 , num_of_hex * sizeof(char));	//init 0
	
	for(i = 0; i < num_of_hex; i++)
	{
		for(j = 0; j < 4; j++)
		{
			int index = (i * 4) + j;
			if(index < length)
				buffer[i] |= (array[index] << j);	
		}
	}
	
	char x;
	
	for(i = num_of_hex - 1; i >= 0 ; i--)
	{
		switch(buffer[i]){
			case 0 : x = '0';  break;
			case 1 : x = '1';  break;
			case 2 : x = '2';  break;
			case 3 : x = '3';  break;	
			case 4 : x = '4';  break;
			case 5 : x = '5';  break;	
			case 6 : x = '6';  break;
			case 7 : x = '7';  break;	
			case 8 : x = '8';  break;
			case 9 : x = '9';  break;
			case 10 : x = 'A'; break;	
			case 11 : x = 'B'; break;
			case 12 : x = 'C'; break;	
			case 13 : x = 'D'; break;
			case 14 : x = 'E'; break;
			case 15 : x = 'F'; break;
			default : break;
 		}	
		printf("%c" , x);		
	}

	printf("\n");
}

void arrayCopy(bit * a , bit * b , int length){
	
	int i;
	
	for(i = 0; i < length; i++)
		b[i] = a[i];
	
}

int arrayEqual(bit *a1 , bit *a2 , unsigned int length){
	
	int i;
	for(i = 0; i < length; i++)
		if(a1[i] != a2[i])
			return -1;
	
	return 0;
		
}

