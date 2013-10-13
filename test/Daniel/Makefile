MYLIB  	= mylib.o
TESTER 	= test.o
CC 	= gcc
CFLAGS 	= -Wall

all: $(MYLIB) $(TESTER)
	$(CC) $(CFLAGS) -o test $(MYLIB) $(TESTER)

clean:
	rm -f *.o test
	
clean_o:	
	rm -f *.o
