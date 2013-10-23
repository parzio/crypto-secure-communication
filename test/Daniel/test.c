
#include "streamChiper.h"
#include "utility.h"


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
	
	char c[5] = { 'c' , 'i' , 'a' , 'o' , '\0' };
	
	printf("ciao : %s \n %d \n" , c , sizeof(c));
	
	/*ALL5 all5;
	
	ALL5_init(&all5);
	
	ALL5_getStream(&all5, result , stream);
	
	printArray(result , stream);	
	
	ALL5_delete(&all5);*/

	return 0;
		
}