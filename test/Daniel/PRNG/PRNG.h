
#include "../StreamChiper/streamChiper.h"
#include "../BlockChiper/Bunny24.h"

#include <openssl/bio.h>
#include <openssl/bn.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>

void initFPRNG();
void FPRNG(bit * result , int length);
void SPRNG(bit * result , int length);
void primeGenerator(bit * number , int length);
