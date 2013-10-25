TESTER 		= test.o
CC 		= gcc
CFLAGS 		= -Wall -w
OBJECT  = $(wildcard StreamChiper/*.o)
OB	= StreamChiper/streamChiper.o StreamChiper/all5.o StreamChiper/maj5.o StreamChiper/utility.o StreamChiper/a51.o StreamChiper/lfsr.o

all: $(OB)  $(TESTER) 
	cd StreamChiper; make
	$(CC) $(CFLAGS) -o test $(OB) $(TESTER)

clean:
	rm -f *.o test $(OBJECT)
	
