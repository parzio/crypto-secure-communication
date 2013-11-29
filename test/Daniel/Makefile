TESTER 		= test.o
CC 		= gcc
CFLAGS 		= -Wall -w
STREAMCHIPER	= $(patsubst %.c, %.o, $(wildcard StreamChiper/*.c))
UTILITY		= $(patsubst %.c, %.o, $(wildcard Utility/*.c))
BLOCKCHIPER	= $(patsubst %.c, %.o, $(wildcard BlockChiper/*.c))
PRNG		= PRNG/FPNRG.o PRNG/SPRNG.o


all: $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(TESTER) 
	$(CC) $(CFLAGS) -o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(TESTER) -lssl -lcrypto 

PRNG/SPRNG.o: PRNG/SPRNG.c 
	$(CC) $(CFLAGS) -c PRNG/SPRNG.c -lssl -lcrypto
	
PRNG/FPRNG.o: PRNG/FPRNG.c
	$(CC) $(CFLAGS) -c PRNG/FPRNG.c 

clean:
	rm -f *.o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG)
	
