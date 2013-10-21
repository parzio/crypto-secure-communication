STREAMCHIPER  	= streamChiper.o
UTILITY		= utility.o
TESTER 		= test.o
CC 		= gcc
CFLAGS 		= -Wall -w

all: $(STREAMCHIPER) $(UTILITY) $(TESTER) 
	$(CC) $(CFLAGS) -o test $(STREAMCHIPER) $(TESTER) $(UTILITY)

clean:
	rm -f *.o test
	
clean_o:	
	rm -f *.o
