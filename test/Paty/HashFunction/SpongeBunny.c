#include <stdio.h>
#include <stdlib.h>

#include "SpongeBunny.h"
#include "../BlockChiper/Bunny24.h"

#define WIDTH		24 //b
#define BITRATE		20 //r
#define CAPACITY	 4 //c

#define PADDING_VALUE 0

static bit KEY[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

char* SPONGEBUNNY(message* m){
	
	int i,j;

	printf("Message received\n");
	printArray(m->value, m->length);
	
	char messageHash[HASH_LENGTH];
	memset(&messageHash, 0, HASH_LENGTH * sizeof(char));

	bit output[HASH_LENGTH];
	memset(&output, 0, HASH_LENGTH * sizeof(bit));

	//Determine the number of absorbtion rounds	
	int rounds = m->length/BITRATE;
	if( m->length % BITRATE != 0)
		rounds++;

	//Prepare the inputs for each round of absorbtion
	bit input[rounds][BITRATE];
	for(i = 0; i < rounds-1; i++)
	{
		for(j = 0; j < BITRATE; j++)
			input[i][j] = m->value[i*BITRATE+j];	
	}	

	if ( (m->length % BITRATE) == 0 )
	{
		for(j = 0; j < BITRATE; j++)
			input[rounds-1][j] = m->value[i*BITRATE+j];
	}
	else
	{//padding the message to be a multiple of r (bitrate)
		for(j = 0; j < m->length % BITRATE; j++)
			input[rounds-1][j] = m->value[(rounds-1)*BITRATE+j];
		for(j = m->length % BITRATE; j < BITRATE; j++)
			input[rounds-1][j] = PADDING_VALUE;
	}

	
	bit state[WIDTH];
	memset(&state, 0, WIDTH * sizeof(bit) );

	//Absorbtion phase

	for(i = 0;i < rounds; i++)
	{
		//XOR
		for(j = 0; j < BITRATE; j++)
			state[j] ^= input[i][j];
	
		//Apply Bunny24	
		bunny24_encrypt(state, KEY);
	}		

	//Sqeezing phase	

	int roundsSqueeze = HASH_LENGTH/BITRATE;

	for(i = 0; i < roundsSqueeze; i++)
	{
		for(j = 0; j < BITRATE; j++ )
			output[i*BITRATE + j] = state[j];

		bunny24_encrypt(state, KEY);
	}

	//TODO: make the output a string 
	printf("Output value: \n");
	printArray(output, HASH_LENGTH);
	return &messageHash;
}



