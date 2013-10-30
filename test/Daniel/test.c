
#include "StreamChiper/streamChiper.h"

int main(int argc, char ** argv){
	
	unsigned int stream = 20;
	//bit result[stream];

	bit key[64] = {0 ,1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0,
		1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 
		0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};

	bit frameVector[22] = {0,0,1,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
	
	byte mess[4] = {'C' , 'I' , 'A' , '0'};
		
	byte chip[4];

	int i;
	for(i = 0; i < 4; i++){
		printf("%c , " , mess[i]);
	}

	return 0;
	
	/*unsigned int degree = 8;
	
	bit pA[] = 		{1 , 1 , 0 , 0 , 0 , 0 , 0 , 0};
	bit pB[] = 		{0 , 0 , 1 , 1 , 0 , 0 , 0 , 0};
	bit result[degree];
	bit field[] = 	{1 , 1 , 1 , 0 , 1 , 0 , 0 , 0 , 1}; 

	arrayMul(pA , pB , field, result , degree);*/
		
}