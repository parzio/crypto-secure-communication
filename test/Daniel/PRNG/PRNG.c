#include "PRNG.h"

void initFPRNG(){
	srand(time(NULL));
}

void FPRNG(bit * result , int length){	//you have to call initFPRNG before calling this fanction

	bit seed1[64] , seed2[64];
	bit init1[22] , init2[22];
	bit maj_buffer[length];
	bit all_buffer[length];
	
	int i;

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

	for(i = 0; i < length; i++)
		result[i] = maj_buffer[i] ^ all_buffer[i];

}

void SPRNG(bit * result , int length){
	
	int cicle = length + (24 - (length % 24));	//cicle must be divisible for 24 
	
	bit fseed[cicle * 2];	//seed for the SPNRG --> i take 48 bits form this array to generate 24 bits with bunny24
	bit resul[cicle];

	FPRNG(fseed , cicle * 2);	//generate the Seed

	int i , end = cicle/24 , p = 0;
	bit key[24] , block[24];
		
	for(i = 0; i < end; i++)	//for each 24 bits
	{	
		int ks = i * 48;
		int ke = ks + 24;
		int bs = ke;
		int be = bs + 24; 
		
		int z = 0 , j;
		
		for(j = ks; j < ke; j++)	//the first 24 bits of the cicle are the key
		{	
			key[z] = fseed[j];
			z++;			
		}

		z = 0;

		for(j = bs; j < be; j++)	//the remaining 24 are the message
		{
			block[z] = fseed[j];
			z++;
		}

		bunny24_encrypt(block , key);

		int rs = i * 24;
		int re = rs + 24;
		z = 0;
						
		for(j = rs; j < re; j++)
		{
			resul[j] = block[z];
			z++;
		}			
	}

	for(i = 0; i < length; i++)
		result[i] = resul[i];

}

void primeGenerator(bit * number , int length){

	BIGNUM *n;
	
	n = BN_new();

	int exit = 0 , i = 0;
	int byteLength = length / 8 + ((length % 8 != 0) ? 1 : 0); //length in byte
	byte res[byteLength];	

	int count = 0;
	
	do{		
		FPRNG(number , length);	//generate a number

		memset(res , 0 , byteLength * sizeof(byte));	//initialize to 0
		
		for(i = 0; i < length; i++)
			res[i/8] |= (number[i] << (i % 8));	
		//from array of bits to array of byte --> i / 8 : current byte m, i % 8 : current bit inside the byte	

		n = BN_bin2bn((const unsigned char *) res, byteLength , NULL);	//to big number
	
		if(BN_is_prime(n ,10, NULL, NULL, NULL))
			exit = 1;

	}while(exit == 0);

}



