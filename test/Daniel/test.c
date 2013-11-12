
#include "StreamChiper/streamChiper.h"
#include "BlockChiper/Bunny24.h"

int main(int argc, char ** argv){
	
	unsigned int stream = 20;
	//bit result[stream];
/*
	bit key[64] = {0 ,1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 
		0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};

	bit frameVector[22] = {0,0,1,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0};*/
	/*
	MAJ5 maj5;
	
	MAJ5_init(&maj5);
	
	MAJ5_keyLoading(&maj5, key, frameVector);

	MAJ5_warmUpStream(&maj5 , 100);
		
	MAJ5_printStatus(&maj5);
	
	MAJ5_getStream(&maj5, result , stream);
	
	printArray(result , stream);
	
	MAJ5_delete(&maj5);
	*/
	/*
	ALL5 all5;
	
	ALL5_init(&all5);
	
	ALL5_keyLoading(&all5, key, frameVector);
	
	ALL5_warmUpStream(&all5 , 100);
	
	ALL5_printStatus(&all5);
	
	ALL5_getStream(&all5, result , stream);
	
	printArray(result , stream);	

	ALL5_delete(&all5);*/
	
	/*byte mess[4] = {'C' , 'I' , 'A' , '0'};
		
	byte chip[4];

	int i;
	for(i = 0; i < 4; i++){
		printf("%c , " , mess[i]);
	}

	return 0;*/
	
	
	
	
	/*bit testbunny[6] = {0 , 1 , 1, 0 , 0 , 1};
	bit result[6];
	
	sbox1(testbunny , result);

	printArray(result , 6);
	
		sbox2(testbunny , result);

	printArray(result , 6);
	
		sbox3(testbunny , result);

	printArray(result , 6);
	
		sbox4(testbunny , result);

	printArray(result , 6);
	/*
	unsigned int degree = 8;
	
	bit pA[] = 		{0 , 1 , 1 , 0 , 0 , 0 , 0 , 0};
	bit pB[] = 		{0 , 1 , 1 , 0 , 0 , 0 , 0 , 0};
	bit result[degree];
	bit field[] = 	{1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 1}; 

	arrayMul(pA , pB , field, result , degree);
	

	printArray(result , degree);*/
	
	char * hex_key = "FB7F50";
	
	bit key[24] = {1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0}; 
	bit plain[24] = {1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0};
	
	char * hex_meg = "8B7846";

	hexToBinary(plain , hex_meg);
	hexToBinary(key , hex_key);
	
	printHex(plain , 24);
	
	bunny24_encrypt(plain , key);

	bunny24_decrypt(plain , key);
	
	printHex(plain , 24);
	
	
	bit k1[6] = {0, 1, 0, 0, 0, 0};
	bit k2[6] = {0, 1, 0, 0, 0, 0};
	bit k3[6] = {0, 1, 0, 0, 0, 0};	
	bit k4[6] = {0, 1, 0, 0, 0, 0};
	
	sbox(k1 , 0);
	sbox(k2 , 1);
	sbox(k3 , 2);
	sbox(k4 , 3);
	
	printArray(k1 , 6);
	printArray(k2 , 6);
	printArray(k3 , 6);
	printArray(k4 , 6);

	sbox(k1 , 0);
	sbox(k2 , 1);
	sbox(k3 , 2);
	sbox(k4 , 3);
	

	
	printArray(k1 , 6);
	printArray(k2 , 6);
	printArray(k3 , 6);
	printArray(k4 , 6);
	
	
	
}