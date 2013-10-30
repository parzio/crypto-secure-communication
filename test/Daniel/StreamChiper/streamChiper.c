#include "streamChiper.h"


void initStreamChiper(streamChiper *chiper , streamChiperSuite type , bit *key , bit *initVector){
		
	chiper->type = type;

	switch(type){
		
		case Stream_A51 : {
			A51_init(&chiper->a51);
			A51_keyLoading(&chiper->a51 , key , initVector);
			A51_warmUpStream(&chiper->a51 , 100);
			break;
		}
		
		case Stream_MAJ5 : {
			MAJ5_init(&chiper->maj5);
			MAJ5_keyLoading(&chiper->maj5 , key , initVector);
			MAJ5_warmUpStream(&chiper->maj5 , 100);
			break;
		}
		
		case Stream_ALL5 : {
			ALL5_init(&chiper->all5);
			ALL5_keyLoading(&chiper->all5 , key , initVector);
			ALL5_warmUpStream(&chiper->all5 , 100);
			break;
		}
		
		default : break;
		
	}
	
}

void deleteStreamChiper(streamChiper *chiper){	
	
	switch(chiper->type){		
		case Stream_A51 : {
			A51_delete(&chiper->a51);
			break;
		}		
		case Stream_MAJ5 : {
			MAJ5_delete(&chiper->maj5);
			break;
		}		
		case Stream_ALL5 : {
			ALL5_delete(&chiper->all5);
			break;
		}	
		default : break;		
	}
}

void encrypt(streamChiper *chiper , byte *plaintext, byte *chipertext, int length){
	
/*	switch(chiper->type){		
		case Stream_A51 : {
			A51_encrypt(&chiper->a51, plaintext, chipertext, (unsigned) length);
			break;
		}		
		case Stream_MAJ5 : {
			MAJ5_encrypt(&chiper->maj5, plaintext, chipertext, (unsigned) length);
			break;
		}		
		case Stream_ALL5 : {
			ALL5_encrypt(&chiper->all5, plaintext, chipertext, (unsigned) length);
			break;
		}	
		default : break;		*/
	//}	
}
void decrypt(streamChiper *chiper , byte *plaintext, byte *chipertext, int length){
	/*
	switch(chiper->type){		
		case Stream_A51 : {
			A51_decrypt(&chiper->a51, plaintext, chipertext, (unsigned) length);
			break;
		}		
		case Stream_MAJ5 : {
			MAJ5_decrypt(&chiper->maj5, plaintext, chipertext, (unsigned) length);
			break;
		}		
		case Stream_ALL5 : {
			ALL5_decrypt(&chiper->all5, plaintext, chipertext, (unsigned) length);
			break;
		}	
		default : break;		*/
//	}			
}

