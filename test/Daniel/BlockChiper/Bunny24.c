
#include "Bunny24.h"

void bunny24_encrypt(bit block[block_length] , bit key[key_length]){
	
	blockInversion(block);
	blockInversion(key);

	bit keys[16][key_length];
	
	keySchedule(key , keys);

	int i;

	arraySum(block , keys[0] , block , block_length); // first XOR

	for(i = 1; i <= 15; i++)					// 15 rounds
		round(block , keys[i]);
	
	blockInversion(block);
	blockInversion(key);
	
}

void bunny24_decrypt(bit block[block_length] , bit key[key_length]){
	
	blockInversion(block);
	blockInversion(key);

	bit keys[16][key_length];
	
	keySchedule(key , keys);

	int i;

	for(i = 15; i > 0; i--)					// 15 rounds
		inverse_round(block , keys[i]);
	
	arraySum(block , keys[0] , block , block_length); // first XOR
	
	blockInversion(block);
	blockInversion(key);
	
}

void keySchedule(bit key[key_length] , bit schedule[16][key_length]){
	
	//Step 1

	bit word[88][6];
	int i , j , r;
	
	for(i = 0; i < 4; i++){
		getChunk(key, word[i], i);
		
		for(j = 0; j < chunk_length; j++)
			word[i + 4][j] = word[i][j];
	}
	
	for(i = 4; i < 8; i++)
	{
		sbox(word[i], i - 4);
		
		if(i < 7)
			arraySum(word[i - 3] , word[i]  , word[i] , chunk_length);
		else
			arraySum(word[0] , word[i]  , word[i] , chunk_length);
		
	}
	
	//step 2

	for(j = 9; j <= 88; j++)
	{
		i = j-1;
		if((j % 8) == 1)
			{
			bit buffer[chunk_length];
			arrayCopy(word[i - 1] , buffer , chunk_length);
			rotate(buffer, -1 , chunk_length);	
			sbox(buffer , 1);
			arraySum(buffer , word[i - 8], word[i], chunk_length);
			arraySum(word[i], keyScheduleArray , word[i], chunk_length);
			}
		else if((j % 8) == 5)
			{
			bit buffer[chunk_length];
			arrayCopy(word[i - 1] , buffer , chunk_length);	
			sbox(buffer , 2);	
			arraySum(word[i - 8] , buffer , word[i] , chunk_length);			
			}
		else if((j % 4) != 1)
			arraySum(word[i - 8] , word[i - 1] , word[i] , chunk_length);
	}
	
	//phase 3

	for(r = 0; r < 3; r++)	//for rect 1,2,3
	{		
		for(j = 0; j < 5; j++) //for key 1...5
		{
			int z = (8 + 20*r) + j;
			int index = (r*5)+j;
			for(i = 0; i < 4; i++)	//for each word of the key
			{
				setChunk(schedule[index] , word[z] , i);
				z += 5;
			}
		}
	}
	
	int z = 68;
	for(i = 0; i < 4; i++)	//last key
	{
		setChunk(schedule[15] , word[z] , i);
		z += 5;
	}
}

void round(bit block[block_length], bit key[key_length]){
	
	sboxLayer(block);
	
	mixingLayer(block);
	
	arraySum(block , key , block , block_length);
			
}

void inverse_round(bit block[block_length], bit key[key_length]){
	
	arraySum(block , key , block , block_length);
		
	inverse_mixingLayer(block);
		
	inverse_sboxLayer(block);
			
}

void mixingLayer(bit block[block_length]){
	
	int i;
	
	bit chunk_buffer[4][chunk_length];
	
	for(i = 0; i < 4; i++)	//for each column
		mixingLayerMatrixMul(block , chunk_buffer[i] , i);
		
	for(i = 0; i < 4; i++)
		setChunk(block , chunk_buffer[i] , i);
	
}

void inverse_mixingLayer(bit block[block_length]){
	
	int i;
	
	bit chunk_buffer[4][chunk_length];
	
	for(i = 0; i < 4; i++)	//for each column
		inverse_mixingLayerMatrixMul(block , chunk_buffer[i] , i);
		
	for(i = 0; i < 4; i++)
		setChunk(block , chunk_buffer[i] , i);
	
}

void inverse_mixingLayerMatrixMul(bit block[block_length] , bit result_chunk[chunk_length], unsigned int column){
	
	int i;
	
	for(i = 0; i < chunk_length; i++)
		result_chunk[i] = 0;

	for(i = 0; i < 4; i++)
	{
		bit chunk[chunk_length];
		getChunk(block , chunk , i);
		
		arrayMul(chunk , inverse_mixingMatrix[i][column] , primitivePoly , chunk , primitivePolyDegree);
		
		arraySum(result_chunk , chunk , result_chunk , chunk_length);	
	}	
}

void mixingLayerMatrixMul(bit block[block_length] , bit result_chunk[chunk_length], unsigned int column){
	
	int i;
	
	for(i = 0; i < chunk_length; i++)
		result_chunk[i] = 0;

	for(i = 0; i < 4; i++)
	{
		bit chunk[chunk_length];
		getChunk(block , chunk , i);
		
		arrayMul(chunk , mixingMatrix[i][column] , primitivePoly , chunk , primitivePolyDegree);
		
		arraySum(result_chunk , chunk , result_chunk , chunk_length);	
	}	
}

void getChunk(bit block[block_length], bit chunk[chunk_length],  unsigned int index){
	
	int j, z = 0 , end = (chunk_length * index + chunk_length) , start = index * chunk_length;
	
	for(j = start; j < end ; j++)
	{
		chunk[z] = block[j];
		z++;
	}	
	
}

void setChunk(bit block[block_length], bit chunk[chunk_length],  unsigned int index){
	
	int j, z = 0 , end = (chunk_length * index + chunk_length) , start = index * chunk_length;
	
	for(j = start; j < end ; j++)
	{
		block[j] = chunk[z];
		z++;
	}	
}

void blockInversion(bit block[block_length]){
	
	int i;
	bit chunk[chunk_length];
	
	for(i = 0; i < 4; i++)
	{
		getChunk(block , chunk , i);
		inverteArray(chunk , chunk_length);
		setChunk(block , chunk , i);			
	}	
}

void sboxLayer(bit block[block_length]){
	
	int i;
	
	for(i = 0; i < 4; i++)
	{
		bit chunk[chunk_length];	
		getChunk(block , chunk , i);
		
		sbox(chunk , i);
		
		setChunk(block , chunk , i);		
	}	
}

void inverse_sboxLayer(bit block[block_length]){
	
	int i;
	
	for(i = 0; i < 4; i++)
	{
		bit chunk[chunk_length];	
		getChunk(block , chunk , i);
		
		inverse_sbox(chunk , i);
		
		setChunk(block , chunk , i);		
	}	
}

void sbox(bit chunk[chunk_length] , int index){
	int i;
	bit result[chunk_length];
	
	for(i = 0; i < chunk_length; i++)
		result[i] = chunk[i];
	
	switch(index){
		case 0: 	
			for( i = 1; i < sbox1_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			break;
			
		case 1:
			for( i = 1; i < sbox2_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			break;
			
		case 2:
			for( i = 1; i < sbox3_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			break;
			
		case 3:
			for( i = 1; i < sbox4_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			result[2] ^= 1;
			break;
			
		default : printf("sbox index error \n"); break;
	}

	for(i = 0; i < chunk_length; i++)
		chunk[i] = result[i];	
	
}

void inverse_sbox(bit chunk[chunk_length] , int index){
	int i;
	bit result[chunk_length];
	
	for(i = 0; i < chunk_length; i++)
		result[i] = chunk[i];
	
	switch(index){
		case 0: 	
			for( i = 1; i < inverse_sbox1_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			break;
			
		case 1:
			for( i = 1; i < inverse_sbox2_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			break;
			
		case 2:
			for( i = 1; i < inverse_sbox3_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			break;
			
		case 3:
			result[2] ^= 1;
			chunk[2] ^= 1;			
			for(i = 1; i < inverse_sbox4_power; i++)
				arrayMul(result , chunk , primitivePoly , result , primitivePolyDegree);
			break;
			
		default : printf("sbox index error \n"); break;
	}

	for(i = 0; i < chunk_length; i++)
		chunk[i] = result[i];	
	
}


