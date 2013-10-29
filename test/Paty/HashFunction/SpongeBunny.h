#ifndef SPONGE_BUNNY
#define SPONGE_BUNNY

#define HASH_LENGTH 160

typedef struct message
{
	int* value;
	int length;
}message;

char* SPONGEBUNNY(message* m);

#endif

