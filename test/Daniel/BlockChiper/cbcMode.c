
#include "cbcMode.h"

int getCipherLength(int length){
	
	return (length / block_length + 1) * block_length; 
		
}

bunny24CBC_encrypt(bit * message ,bit * ciphertext , bit initVector[block_length] , bit key[block_length] , unsigned int length){
	
	int num = length / block_length + 1 , i;
	
	bit blocks[num][block_length];
	
	messageToBlocks(message , blocks , length);

	arraySum(blocks[0] , initVector , blocks[0] , block_length);
		
	for(i = 0; i < num; i++)
	{
		bunny24_encrypt(blocks[i] , key);
		
		if(i < (num-1))
			arraySum(blocks[i] , blocks[i + 1] , blocks[i + 1] , block_length);

	}
	
	blocksToMessage(ciphertext , blocks , getCipherLength(length));

}

void messageToBlocks(bit * message , bit blocks[][block_length] , unsigned int length){

	int i , j;
	
	for(i = 0; i < length; i++)
	{
		j = i / block_length;
		blocks[j][i % block_length] = message[i];				
	}
	
	int end = getCipherLength(length);
	
	for(i = length; i < end; i++)
	{
		j = i / block_length;
		blocks[j][i % block_length] = 0;				
	}

}

void blocksToMessage(bit * message , bit blocks[][block_length] , unsigned int length){

	int i , j;
	
	for(i = 0; i < length; i++)
	{
		j = i / block_length;
		message[i] = blocks[j][i % block_length];				
	}
	
}

void bunny24CBC_decrypt(bit * message , bit * ciphertext, bit initVector[block_length] , bit key[block_length] , unsigned int length){
	
	int num = length / block_length + 1 , i;
	
	bit blocks[num][block_length];
	
	messageToBlocks(ciphertext , blocks , getCipherLength(length));
		
	for(i = num - 1; i >= 0; i--)
	{
	
		bunny24_decrypt(blocks[i] , key);
		
		if(i > 0)
			arraySum(blocks[i - 1] , blocks[i] , blocks[i] , block_length);
		
	}

	arraySum(blocks[0] , initVector , blocks[0] , block_length);
	
	blocksToMessage(message , blocks , length);
	
}
