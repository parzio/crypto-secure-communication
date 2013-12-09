
#include "../StreamChiper/streamChiper.h"

/*
 * TODO BE AWARE : the array are stored in bigEndian inverse format so 0x123456 is stored [65 , 43 , 21] (in bytes) 
 * 
 */

typedef struct array_of_bit{
	bit *array; 	
	int length;
} bitArray;

/* utility */

bitArray * newEmptyBitArray();

bitArray * newBitArray(const int l);

void resizeBitArray(bitArray * bA , const int l);

void deleteBitArray(bitArray * bA);

void copyBitArray(bitArray * r , const bitArray * s);

/* print bitArray */

void printBinaryBitArray(const bitArray *bA);

void printHexBitArray(const bitArray *bA);

/* other --> bitArray */

void hex2BitArray(bitArray *r , const char * hex);

void binary2BitArray(bitArray *r, const bit[], const int length);

void byte2BitArray(bitArray *r, const byte[], const int length);

/* bitArray --> other */

void bitArray2Hex(const bitArray *a , char * hex);

void bitArray2Binary(const bitArray *a, bit dest[]);

void bitArray2Byte(const bitArray *a, byte dest[]);

/* functions */

int getGradeBitArray(const bitArray *a);

int cmpBitArray(const bitArray *a , const bitArray *b);

void sumBitArray(bitArray *r , const bitArray * a , const bitArray * b);

void mul_mod_BitArray(bitArray *r , const bitArray * a , const bitArray * b , const bitArray * n);

void shiftBitArray( bitArray * a , const int shift);	// shift > 0 --> right shift , else left

