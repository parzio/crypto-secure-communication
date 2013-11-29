
#include "FPRNG.h"
#include "SPRNG.h"

void SPRNG(bit * result , int length){
	
	int cicle = length + (24 - (length % 24));	// 
	
	bit fseed[cicle * 2];
	bit resul[cicle];

	FPRNG(fseed , cicle * 2);

	int i , end = cicle/24 , p = 0;

	for(i = 0; i < end; i++)	//for each 24 bits
	{
		bit key[24] , block[24];
		
		int ks = i * 48;
		int ke = ks + 24;
		int bs = ke;
		int be = bs + 24; 
		
		int z = 0 , j;
		
		for(j = ks; j < ke; j++)
		{	
			key[z] = fseed[j];
			z++;		
			
		}

		z = 0;

		for(j = bs; j < be; j++)
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
	
	int n = 0;
	
	for(i = 0; i < length; i++)
	{
		result[i] = resul[i];
		if(result[i] == 1)
			n++;
	}
	
	printf("num --> %f \n \n " ,(double) n / (double) length);
}

void primeGenerator(bit * number , int length){

  BIGNUM *n;
 
  int is_p ;
 
  n = BN_new() ;
/*
  if (BN_rand( n, 2, 1, 10) == 1) { 
    printf("n = %s\n", BN_bn2hex(n) ) ; // print as char of hexadecimal character
    printf("n = %s\n", BN_bn2dec(n) ) ; // print as char of decimal     character
  }
	
	int i = 0;
	
	//do{
		if(BN_is_prime(n,10,NULL, NULL, NULL))
			printf("prime!!!");
		else
			printf("not prime!!!");
		
	//}while(i = 0);
	*/

	
}



