#include <stdio.h>
#include <stdlib.h>

#include "SpongeBunny.h"

void spongeBunnyComputeHash(byte * message , byte hash[HASH_BYTE_LENGTH], int length){
	
	int i,j;

	memset(hash, 0, HASH_BYTE_LENGTH * sizeof(byte));
	
	int bitLength = length * 8;
	bit bit_message[bitLength];
	
	for(i = 0; i < bitLength; i++)
		bit_message[i] = (message[i / 8] & (1 << i % 8)) ? 1 : 0;	// to bit
	

	bit output[HASH_BIT_LENGTH];
	memset(output, 0, HASH_BIT_LENGTH * sizeof(bit));

	//Determine the number of absorbtion rounds	
	int rounds = bitLength/BITRATE;
	if(bitLength % BITRATE != 0)
		rounds++;

	//Prepare the inputs for each round of absorbtion
	bit input[rounds][BITRATE];
	for(i = 0; i < rounds-1; i++)
	{
		for(j = 0; j < BITRATE; j++)
			input[i][j] = bit_message[i*BITRATE+j];	
	}	

	if((bitLength % BITRATE) == 0 )
	{
		for(j = 0; j < BITRATE; j++)
			input[rounds-1][j] = bit_message[i*BITRATE+j];
	}
	else
	{//padding the message to be a multiple of r (bitrate)
		for(j = 0; j < bitLength % BITRATE; j++)
			input[rounds-1][j] = bit_message[(rounds-1)*BITRATE+j];
		for(j = bitLength % BITRATE; j < BITRATE; j++)
			input[rounds-1][j] = PADDING_VALUE;
	}

	
	bit state[WIDTH];
	memset(&state, 0, WIDTH * sizeof(bit));

	//Absorbtion phase

	for(i = 0;i < rounds; i++)
	{
		//XOR
		for(j = 0; j < BITRATE; j++)
			state[j] ^= input[i][j];
	
		//Apply Bunny24	
		bunny24_encrypt(state, SPONGE_KEY);
	}		

	//Sqeezing phase	

	int roundsSqueeze = HASH_BIT_LENGTH/BITRATE;

	for(i = 0; i < roundsSqueeze; i++)
	{
		for(j = 0; j < BITRATE; j++ )
			output[i*BITRATE + j] = state[j];

		bunny24_encrypt(state, SPONGE_KEY);
	}

		for(i = 0; i < HASH_BIT_LENGTH; i++)
			hash[i/8] |= (output[i] << (i % 8));	
	
	
}



