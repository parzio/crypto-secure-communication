#ifndef _CBCMODE_H
#define _CBCMODE_H

#include "Bunny24.h"

void messageToBlocks(bit * message , bit blocks[][block_length] , unsigned int length);
void blocksToMessage(bit * message , bit blocks[][block_length] , unsigned int length);

int  getCipherLength(int length);

void bunny24CBC_encrypt(bit * message, bit * ciphertext , bit initVector[block_length] , bit key[block_length] , unsigned int length);
void bunny24CBC_decrypt(bit * message, bit * ciphertext , bit initVector[block_length] , bit key[block_length] , unsigned int length);

#endif
