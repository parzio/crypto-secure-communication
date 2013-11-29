
#include "../StreamChiper/streamChiper.h"
#include "../BlockChiper/Bunny24.h"

#include <openssl/bio.h>
#include <openssl/bn.h>
#include <stdio.h>

void SPRNG(bit * result , int length);
void primeGenerator(bit * number , int length);
