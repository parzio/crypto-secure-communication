SERVER_OBJS=server.o Utils/common.o
CLIENT_OBJS=client.o Utils/common.o
CC=gcc
CFLAGS=-Wall

all: server client

client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@

server: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $@

clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) server client
