
#include "StreamChiper/streamChiper.h"
#include "BlockChiper/cbcMode.h"

int main(int argc, char ** argv){
	
	unsigned int stream = 20;
	//bit result[stream];

	bit msg[64] = {0 ,1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 
		0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};

	bit key[24];
	bit vector[24];

	char * initvector = "952795";
	char * hex_key = "551458";
	
	/*hexToBinary(vector , initvector);	
	hexToBinary(key , hex_key);
	
	bit ciphertext[getCipherLength(64)];
	
	printHex(msg , 64);
	
	bunny24CBC_encrypt(msg , ciphertext , vector , key , 64);
	
	printHex(ciphertext , 72);
	
	bunny24CBC_decrypt(msg , ciphertext , vector , key , 64);
	
	printHex(msg , 64);
	
	*/
	bit g[1000000];
	
	FPRNG(g , 1000000);
	
	//SPRNG(g , 100000);
	
	//printArray(g , 10000);
	
	
	
	

}