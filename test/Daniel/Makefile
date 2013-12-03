TESTER 		= test.o
CC 		= gcc
CFLAGS 		= -Wall -w
STREAMCHIPER	= $(patsubst %.c, %.o, $(wildcard StreamChiper/*.c))
UTILITY		= $(patsubst %.c, %.o, $(wildcard Utility/*.c))
BLOCKCHIPER	= $(patsubst %.c, %.o, $(wildcard BlockChiper/*.c))
PRNG		= $(patsubst %.c, %.o, $(wildcard PRNG/*.c))
SERVER_OBJS=server.o
CLIENT_OBJS=client.o

all: server client 

client: $(CLIENT_OBJS) $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $@ $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) -lcrypto

server: $(SERVER_OBJS) $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $@ $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) -lcrypto
	
ss: $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(TESTER) server client
	$(CC) $(CFLAGS) -o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(TESTER) -lcrypto
	
clean:
	rm -f *.o test $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(SERVER_OBJS) $(CLIENT_OBJS) server client
	
