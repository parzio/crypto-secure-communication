#ifndef SPONGE_BUNNY
#define SPONGE_BUNNY

#define HASH_BIT_LENGTH 	160
#define HASH_BYTE_LENGTH	HASH_BIT_LENGTH / 8
#include "../Utility/utility.h"
#include "../BlockChiper/Bunny24.h"

#define WIDTH		24 //b
#define BITRATE		20 //r
#define CAPACITY	 4 //c
#define PADDING_VALUE 0

static bit SPONGE_KEY[24] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

void spongeBunnyComputeHash(byte * message , byte hash[HASH_BYTE_LENGTH] , int length);

#endif

