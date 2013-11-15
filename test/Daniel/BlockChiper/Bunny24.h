#ifndef _BUNNY24_H
#define _BUNNY24_H

#include "../Utility/utility.h"

#define block_length 24
#define key_length 24
#define chunk_length 6
										
static bit primitivePoly[] = {1 , 1 , 0 , 1 , 1 , 0 , 1};
static unsigned int primitivePolyDegree = 6;

static unsigned int sbox1_power = 62;
static unsigned int sbox2_power = 5;
static unsigned int sbox3_power = 17;
static unsigned int sbox4_power = 62;

static unsigned int inverse_sbox1_power = 62;
static unsigned int inverse_sbox2_power = 38;
static unsigned int inverse_sbox3_power = 26;
static unsigned int inverse_sbox4_power = 62;


static bit keyScheduleArray[] = {0 , 1 , 0 , 1 , 0 , 1};

static bit mixingMatrix[4][4][6] = {
	{	{1 , 1 , 0 , 0 , 0 , 1} , {1 , 1 , 0 , 1 , 1 , 1} , {0 , 0 , 0 , 1 , 1 , 1} , {1 , 0 , 1 , 1 , 1 , 1} } , //row1
	{	{1 , 0 , 1 , 1 , 0 , 0} , {0 , 0 , 1 , 1 , 1 , 1} , {0 , 1 , 1 , 0 , 1 , 0} , {0 , 0 , 0 , 1 , 1 , 0} } , //row2
	{	{1 , 1 , 0 , 0 , 0 , 0} , {0 , 0 , 0 , 0 , 0 , 1} , {1 , 1 , 1 , 0 , 1 , 0} , {1 , 1 , 1 , 0 , 1 , 1} } , //row3
	{	{0 , 0 , 1 , 1 , 0 , 1} , {0 , 1 , 1 , 0 , 0 , 1} , {0 , 0 , 0 , 1 , 1 , 1} , {1 , 1 , 0 , 0 , 1 , 0} }}; //row4
	
static bit inverse_mixingMatrix[4][4][6] = {
	{	{1 , 0 , 1 , 1 , 1 , 0} , {1 , 1 , 0 , 0 , 0 , 0} , {1 , 1 , 0 , 1 , 0 , 0} , {1 , 0 , 0 , 1 , 1 , 0}	}	,
	{	{1 , 0 , 0 , 0 , 1 , 0} , {1 , 1 , 1 , 1 , 0 , 1} , {0 , 1 , 1 , 1 , 1 , 1} , {1 , 0 , 1 , 1 , 1 , 1}	}	,
	{	{1 , 1 , 1 , 0 , 0 , 0} , {1 , 1 , 1 , 0 , 1 , 0} , {1 , 0 , 0 , 1 , 1 , 1} , {0 , 0 , 1 , 1 , 0 , 0}	}	,
	{	{0 , 1 , 0 , 1 , 0 , 0} , {0 , 1 , 0 , 1 , 1 , 1} , {1 , 0 , 1 , 1 , 0 , 0} , {1 , 0 , 0 , 1 , 0 , 1}	}};



void getChunk(bit block[block_length], bit chunk[chunk_length],  unsigned int index);	//obtain the ith chunk of the block
void setChunk(bit block[block_length], bit chunk[chunk_length],  unsigned int index); 
void blockInversion(bit block[block_length]);



void keySchedule(bit key[key_length], bit schedule[16][key_length]);


void bunny24_encrypt(bit block[block_length], bit key[key_length]);
void round(bit block[block_length], bit key[key_length]);
void sboxLayer(bit block[block_length]);
void sbox(bit chunk[chunk_length] , int index);
void mixingLayer(bit block[block_length]);
void mixingLayerMatrixMul(bit block[block_length] , bit result_chunk[chunk_length], unsigned int column);

void bunny24_decrypt(bit block[block_length], bit key[key_length]);
void inverse_round(bit block[block_length], bit key[key_length]);
void inverse_mixingLayer(bit block[block_length]);
void inverse_mixingLayerMatrixMul(bit block[block_length] , bit result_chunk[chunk_length], unsigned int column);
void inverse_sboxLayer(bit chunk[chunk_length]);
void inverse_sbox(bit chunk[chunk_length] , int index);

void bunny24_bruteForce(bit plain[block_length] , bit crypt[block_length]);

#endif
