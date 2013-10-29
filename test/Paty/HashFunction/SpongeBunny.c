#include "SpongeBunny.h"
#include "../BlockChiper/Bunny24.h"

#define WIDTH		24 //b
#define BITRATE		20 //r
#define CAPACITY	 4 //c

#define PADDING_VALUE 0

char* SPONGEBUNNY(message* m)
{

	char messageHash[HASH_LENGTH];
	memset(&messageHash, 0, HASH_LENGTH * sizeof(char));
	
	int rounds = m->length/BITRATE;

	int i,j;

	if( m->length % BITRATE != 0)
		rounds++;
	
	int input[rounds][BITRATE];

	for(i = 0; i < rounds-1; i++)
	{
		for(j = 0; j < BITRATE; j++)
			input[i][j] = m->value[i*BITRATE+j];
	}	

	if (m->length % BITRATE == 0 ){
				
		for(j = 0; j < BITRATE; j++)
			input[rounds-1][j] = m->value[i*BITRATE+j];
	}
	else{
		
		//padding the message to be a multiple of b (width)
		for(j = 0; j < m->length % BITRATE; j++)
			input[rounds-1][j] = m->value[i*WIDTH+j];
		for(j = m->length % BITRATE; j < WIDTH; j++)
			input[rounds-1][j] = PADDING_VALUE;
	}
	printArray(m->value, WIDTH);
	printArray(input[0],WIDTH);

	//Absorbtion phase

	int state[WIDTH];


	bit key[24];
	char * hex_key = "551458";
	hexToBinary(key , hex_key);

	memset(&state, 0, WIDTH * sizeof(int) );


	for(i = 0;i < rounds; i++)
	{
		//XOR
		for(j = 0; j < BITRATE; j++)
			state[j] ^= input[i][j];
	
		//Apply Bunny24	

		bunny24_encrypt(state, key);
	}		

	printf("\n");
	printArray(state, 24);	

	


	return &messageHash;
}



