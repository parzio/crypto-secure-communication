
#include "mylib.h"


int main(int argc, char ** argv){
	
	bit p[4] = {1,0,1,1}; 
	bit reg[3] = {1,0,1};
	bit result[10];

	LFSR ciao = { NULL , NULL , 0 };
	
	LFSR_init(&ciao , reg , p , 3);
	
	LFSR_getStream(&ciao , result , 10);
	
	printArray(result , 10);
		
	LFSR_delete(&ciao);
	
	return 0;
		
}