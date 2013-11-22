
#include "FPRNG.h"
#include<stdlib.h>
#include<time.h>

void FPRNG(bit * result , int length){

	bit seed1[64] , seed2[64];
	bit init1[22] , init2[22];
	bit maj_buffer[length];
	bit all_buffer[length];
	
	int i;
	
	srand (time(NULL));
	
	for(i = 0; i < 64; i++){
		seed1[i] = (int) rand() / (int) (RAND_MAX/2);			
		seed2[i] = (int) rand() / (int) (RAND_MAX/2);
	}
	
	for(i = 0; i < 22; i++){
		init1[i] = (int) rand() / (int) (RAND_MAX/2);			
		init2[i] = (int) rand() / (int) (RAND_MAX/2);
	}

	MAJ5 maj5;
	MAJ5_init(&maj5);
	MAJ5_keyLoading(&maj5 , seed1 , init1);
	MAJ5_warmUpStream(&maj5 , 100);
	MAJ5_getStream(&maj5, maj_buffer, (unsigned) length);
	MAJ5_delete(&maj5);
	
	ALL5 all5;
	ALL5_init(&all5);
	ALL5_keyLoading(&all5 , seed2 , init2);
	ALL5_warmUpStream(&all5 , 100);
	ALL5_getStream(&all5, all_buffer , (unsigned) length);
	ALL5_delete(&all5);

	int num = 0;
	
	for(i = 0; i < length; i++){
		result[i] = maj_buffer[i] ^ all_buffer[i];
		if(result[i] == 1)
			num++;
	}
	
	printf("num --> %f \n \n " ,(double) num / (double) length);

}
