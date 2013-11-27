#ifndef SPONGE_BUNNY
#define SPONGE_BUNNY

#define HASH_LENGTH 160
#include "../Utility/utility.h"

typedef struct message
{
	bit* value;
	int length;
}message;

char* SPONGEBUNNY(message* m);

#endif

