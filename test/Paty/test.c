
#include "StreamChiper/streamChiper.h"
#include "BlockChiper/cbcMode.h"
#include "HashFunction/SpongeBunny.h"

int main(int argc, char ** argv){
	
	unsigned int stream = 20;
	//bit result[stream];

/*	bit msg[64] = {0 ,1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 
		0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
*/
	//bit msg[10] = {1, 0, 1, 1, 0, 0, 1, 1, 0, 0};
	bit msg[50] = 	{0,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,
		    	 1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,0,1,0,
			 1,1,1,1,1,1,0,0,1,1};
	message m;
	
	m.value = msg;
	m.length = 50;

	printArray(m.value, 50);
	SPONGEBUNNY(&m);


	//char * hex_key = "551458";
	//char * hex_key = "100000";
	
	//bit key[24];
	//bit vector[24];
	
	//char * hex_meg = "952795";
	/*char * hex_meg = "000000";*/
	
	//printHex(plain , 24);
	/*printHex(key , 24);*/

/*	bunny24_encrypt(plain , key);

	printHex(plain , 24);

	bunny24_decrypt(plain , key);
	
	printHex(plain , 24);*/

	/*
	char * initvector = "952795";
	char * hex_key = "551458";
	
	hexToBinary(vector , initvector);	
	hexToBinary(key , hex_key);
	
	bit ciphertext[getCipherLength(64)];
	
	printHex(msg , 64);
	
	bunny24CBC_encrypt(msg , ciphertext , vector , key , 64);
	
	printHex(ciphertext , 72);
	
	bunny24CBC_decrypt(msg , ciphertext , vector , key , 64);
	
	printHex(msg , 64);
	*/
	/*
	bit testplain[24] = {0 ,1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0};
	
	arraySum(testplain , initvector , testplain , 24);
	
	bunny24_encrypt(testplain , key);
	
	printHex(testplain , 24);
*/
}
