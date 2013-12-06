
#include "StreamChiper/streamChiper.h"
#include "BlockChiper/cbcMode.h"
#include "PRNG/PRNG.h"


int main(int argc, char ** argv){
	
	int rsaKeyLength = atoi(argv[1]);
	int rsaByteKeyLength = rsaKeyLength / 8;
	int primeBitLength 	= rsaKeyLength / 2;
	int primeByteLength	= primeBitLength / 8;
	
	int i;
	
	printf("rsa length : %d \n" , rsaKeyLength);
	
	bit	P[primeBitLength] , Q[primeBitLength];
	
	initFPRNG();
	primeGenerator(P , primeBitLength);
	primeGenerator(Q , primeBitLength);

	byte Pbyte[primeByteLength] , Qbyte[primeByteLength];
	
	memset(Pbyte , 0 , primeByteLength * sizeof(byte));
	memset(Qbyte , 0 , primeByteLength * sizeof(byte));
	
	for(i = 0; i < primeBitLength; i++)
	{
		Pbyte[i/8] |= (P[i] << (i % 8));		//to byte*/
		Qbyte[i/8] |= (Q[i] << (i % 8));		//to byte*/ 
	}
	
	BIGNUM *bN , *bQ , *bP, *pi , *bE , *bD , *gcd;

	bN = BN_new();
	bQ = BN_new();
	bP = BN_new();
	pi = BN_new();
	bE = BN_new();
	bD = BN_new();
	gcd = BN_new();
	
	bQ = BN_bin2bn((const unsigned char *) Qbyte, primeByteLength , NULL);
	bP = BN_bin2bn((const unsigned char *) Pbyte, primeByteLength , NULL);

	BN_CTX * t = BN_CTX_new();

	BN_mul(bN, bP, bQ, t);
	
	BN_sub_word(bQ , 1);
	BN_sub_word(bP , 1);
	
	BN_mul(pi, bP, bQ, t);
	
	
	bit 	bitE[rsaKeyLength];
	byte 	byteE[rsaByteKeyLength];
	
	memset(byteE , 0 , sizeof(byte) * rsaByteKeyLength);
	
	do{
		FPRNG(bitE, rsaKeyLength - 8);	//generate E
									
		for(i = 0; i < rsaKeyLength - 8; i++)
			byteE[i/8] |= (bitE[i] << (i % 8));		//to byte*/
		
		bE = BN_bin2bn((const unsigned char *) byteE, rsaByteKeyLength -1 , NULL);

		BN_gcd(gcd , bE, pi, t);			//gcd	
		
	}while(!(BN_is_one(gcd) && (BN_ucmp(bE, pi) < 0)));
	
	BN_mod_inverse(bD, bE, pi, t);

	byte mess[8] = { 1 , 2 , 3 , 4 , 5 , 6 , 7};
	
	BIGNUM *msg = BN_new();
	
	printf("module  : %s\n", BN_bn2hex(bN));
	printf("pi : %s\n", BN_bn2hex(pi));	
	printf("bE <= pi: %d\n", BN_ucmp(bE, pi));
	printf("public  : %s\n", BN_bn2hex(bE));	
	printf("private : %s\n", BN_bn2hex(bD));
	
	msg = BN_bin2bn((const unsigned char *) mess, 8 , NULL);
	
	printf("plain : %s\n", BN_bn2hex(msg));	
	
	BN_mod_exp(msg , msg, bE , bN , t);
	
	printf("plain : %s\n", BN_bn2hex(msg));
	
	BN_mod_exp(msg , msg, bD , bN , t);
	
	printf("plain : %s\n", BN_bn2hex(msg));
	
	BN_CTX_free(t);
	BN_free(bN);
	BN_free(bQ);
	BN_free(bP);
	BN_free(pi);
	BN_free(bE);
	BN_free(bD);
	BN_free(gcd);
	
	
	return 0;
	
}
