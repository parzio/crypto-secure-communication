
#include "arrays.h"

bitArray * newEmptyBitArray(){
		bitArray * bA = malloc(sizeof(bitArray));	//register	
		bA->length = 0;
		bA->array = NULL;
		return bA;
}

bitArray * newBitArray(const int l){
		bitArray * bA = malloc(sizeof(bitArray));	//register	
		bA->length = l;
		bA->array = malloc(sizeof(bit) * l);	
		memset(bA->array , 0 , sizeof(bit) * l);
		return bA;
}

void resizeBitArray(bitArray * bA , const int l){
		
		if(bA->array != NULL)
			free(bA->array);
	
		bA->length = l;
		bA->array = malloc(sizeof(bit) * l);
		
		memset(bA->array , 0 , sizeof(bit) * l);
}

void deleteBitArray(bitArray * bA){
		if(bA != NULL)
			if(bA->array != NULL)
				free(bA->array);
			free(bA);				
}

void copyBitArray(bitArray * r , const bitArray * s){
	
	if(r->array != NULL)
		free(r->array);
	
	r->length = s->length;
	r->array = malloc(sizeof(bit) * s->length);
	
	memcpy(r->array , s->array , sizeof(bit) * s->length);	
}

void bitArray2Hex(const bitArray *a , char * hex){
	
	int num_of_char = a->length / 4;
	int i , j; 
	char x;
	
	for(i = num_of_char - 1; i >= 0; i--)	//for each char
	{
		int num = 0;
		int e = i * 4 + 4;
		int z = 0;
		
		for(j = (i * 4); j < e; j++)	//for each bit of the char
			num |= (a->array[j] << z++); 
		
		switch(num){
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
 		hex[num_of_char - 1 - i] = x;	
	}
}

void bitArray2Binary(const bitArray *a, bit dest[]){
	memcpy(dest , a->array , sizeof(bit) * a->length);	
}

void bitArray2Byte(const bitArray *a, byte dest[]){
	int i;
	memset(dest , 0 , sizeof(byte) * (a->length / 8));

	for(i = 0; i < a->length; i++)
		dest[i/8] |= (a->array[i] << (i % 8));
	
}

void binary2BitArray(bitArray *r, const bit x[], const int length){	
	resizeBitArray(r , length);	
	memcpy(r->array , x , sizeof(bit) * length); 	
}

void byte2BitArray(bitArray *r, const byte x[], const int length){	
	int l = length * 8;	//bits
	resizeBitArray(r , l);

	int i;
	for(i = 0; i < l; i++)
		r->array[i] = (x[i/8] & (1 << i % 8)) ? 1 : 0;	
}

void hex2BitArray(bitArray *r, const char * hex){
	
	int len = strlen(hex);
	int l   = len * 4 , i , j = 0 , z = 0;
	resizeBitArray(r , l);
	
	for(i = len-1; i >= 0; i--)	//for each char
	{
		byte num;
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
			default : break;
 		}
 		for(j = 0 ; j < 4; j++)
			r->array[z++] = (num & (1 << j)) ? 1 : 0;	
	}
	
}

void printBinaryBitArray(const bitArray *bA){
	
	int i;
	printf("{");
		
	for(i = 0; i < bA->length; i++)
		printf(" %d", bA->array[i]);
	
	printf(" } : %d \n", bA->length);	
}

void printHexBitArray(const bitArray *bA){
	
	if(bA->length % 4 != 0)
	{
		printf("The array needs to be a multiple of 4 (Beta)\n\n");
		return;
	}
	
	int l = bA->length / 4;	
	char str[l];
	
	bitArray2Hex(bA , str);
	
	int i;
	printf("{ ");
		
	for(i = 0; i < l; i++)
		printf("%c" , str[i]);
	
	printf(" } : %d \n", bA->length);

}

int getGradeBitArray(const bitArray *a){
	
	int i = a->length;	//max grade + 1
	do{
		i --;
	}while(a->array[i] != 1);
	
	return i;
}

int cmpBitArray(const bitArray *a , const bitArray *b){
	
	int aG = getGradeBitArray(a);
	int bG = getGradeBitArray(b);
	
	if(aG > bG)
		return 1;	// a > b
	
	if(aG < bG)
		return -1;	//b > a
	
	int i;
	
	for(i = aG - 1; i >= 0; i--)	//for each bit
		if(a->array[i] != b->array[i])
			return (a->array[i] > b->array[i]) ? 1 : -1;
	
	return 0;	//equal	
}

void sumBitArray(bitArray *r , const bitArray * a , const bitArray * b){
		
		int l = (a->length > b->length) ? a->length : b->length;
		
		bitArray * tmp = newBitArray(l);
		
		if(a->length > b->length)
			memcpy(tmp->array , a->array , sizeof(bit) * l);
		else
			memcpy(tmp->array , b->array , sizeof(bit) * l);
		
		int i , e = (a->length < b->length) ? a->length : b->length;
		
		for(i = 0; i < e; i++)
			tmp->array[i] = a->array[i] ^ b->array[i];

		copyBitArray(r , tmp);
		
		deleteBitArray(tmp);
}

void shiftBitArray(bitArray * a , const int shift){
			
		bit tmp[a->length];
		memcpy(tmp , a->array, sizeof(bit) * a->length);

		if(shift > 0){
			int len = abs(shift % a->length);
			memcpy(a->array , tmp + (sizeof(bit) * (a->length - len)) , sizeof(bit) * len);	//copy of the shifted bits
			memcpy(a->array + (sizeof(bit) * len) , tmp , sizeof(bit) * (a->length - len));
		}		
		else if(shift < 0){
			int len = abs(shift % a->length);
			memcpy(a->array , tmp + (sizeof(bit) * len) , sizeof(bit) * (a->length - len));	//copy bits
			memcpy(a->array + (sizeof(bit) * (a->length - len)) , tmp , sizeof(bit) *  len);	//new bits
		}
	
}






