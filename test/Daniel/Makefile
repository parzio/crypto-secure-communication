RSAGEN 		= RSAkeygen.o
CORR 		= correlation.o
CC 		= gcc
CFLAGS 		= -Wall -w
STREAMCHIPER	= $(patsubst %.c, %.o, $(wildcard StreamChiper/*.c))
UTILITY		= $(patsubst %.c, %.o, $(wildcard Utility/*.c))
BLOCKCHIPER	= $(patsubst %.c, %.o, $(wildcard BlockChiper/*.c))
PRNG		= $(patsubst %.c, %.o, $(wildcard PRNG/*.c))
SERVER_OBJS=server.o
CLIENT_OBJS=client.o

all: server client rsakeygen correlation

client: $(CLIENT_OBJS) $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $@ $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) -lcrypto

server: $(SERVER_OBJS) $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $@ $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) -lcrypto
	
rsakeygen: $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(RSAGEN) 
	$(CC) $(CFLAGS) -o rsagen $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(RSAGEN) -lcrypto
	
correlation: $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(CORR) 
	$(CC) -Wall -o correlation $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(CORR) -lcrypto		
	
clean:
	rm -f *.o rsagen correlation $(RSAGEN) $(CORR) $(STREAMCHIPER) $(UTILITY) $(BLOCKCHIPER) $(PRNG) $(SERVER_OBJS) $(CLIENT_OBJS) server client
	
