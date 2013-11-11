
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
	
	bit key[] = {1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0}; 
	bit plain[] = {1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0};
	
	char * hex = "8B7846";
	
	printf("hex --> %X \n \n" , hex);
	
	bit array[24];
	
	hexToBinary(array , hex , 6);
	
	printArray(array , 24);
/*
	bunny24_encrypt(plain , key);
	
	printArray(plain , 24);
*/
	
	

}