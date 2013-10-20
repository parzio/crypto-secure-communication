
#include "mylib.h"


int main(int argc, char ** argv){
	
	unsigned int degree = 11;
	unsigned int stream = 20; 
	
	bit p[] =   {1,0,1,0,0,0,0,0,0,0,0,1}; 
	bit reg[] = {1,0,1,1,1,1,0,0,0,1,1};
	bit result[100];

	LFSR ciao = { NULL , NULL , 0 };
	
	LFSR_init(&ciao , reg , p , degree);
	
	LFSR_getStream(&ciao , result , stream);
	
	printArray(result , stream);
		
	LFSR_delete(&ciao);
	
	MAJ5 maj5;
	
	MAJ5_init(&maj5);
	
	MAJ5_getStream(&maj5, result , stream);
	
	printArray(result , stream);	
	
	MAJ5_delete(&maj5);	

	return 0;
		
}