TESTER 		= test.o
CC 		= gcc
CFLAGS 		= -Wall -w
STREAMCHIPER	= $(patsubst %.c, %.o, $(wildcard StreamChiper/*.c))
UTILITY		= $(patsubst %.c, %.o, $(wildcard Utility/*.c))
BLOCKCHIPER	= $(patsubst %.c, %.o, $(wildcard BlockChiper/*.c))
PRNG		= $(patsubst %.c, %.o, $(wildcard PRNG/*.c))

all: $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(TESTER) 
	$(CC) $(CFLAGS) -o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(TESTER)

clean:
	rm -f *.o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER)
	
