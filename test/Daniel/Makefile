TESTER 		= test.o
CC 		= gcc
CFLAGS 		= -Wall -w
STREAMCHIPER	= $(patsubst %.c, %.o, $(wildcard StreamChiper/*.c))
UTILITY		= $(patsubst %.c, %.o, $(wildcard Utility/*.c))
BLOCKCHIPER	= $(patsubst %.c, %.o, $(wildcard BlockChiper/*.c))

all: $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(TESTER) 
	$(CC) $(CFLAGS) -o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(TESTER)

clean:
	rm -f *.o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER)
	
