TESTER 		= test.o
CC 		= gcc
CFLAGS 		= -Wall -w
STREAMCHIPER	= $(patsubst %.c, %.o, $(wildcard StreamChiper/*.c))

all: $(STREAMCHIPER)  $(TESTER) 
	$(CC) $(CFLAGS) -o test $(STREAMCHIPER) $(TESTER)

clean:
	rm -f *.o test $(STREAMCHIPER)
	
