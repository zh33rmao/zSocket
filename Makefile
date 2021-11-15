CC=gcc
CFLAGS=-g
BINS=server

# all: $(BINS)
# %: %:.c

server: server.c server.h
	$(CC) server.c $(CFLAGS) -o server -lpthread

client: client.c client.h
	$(CC) client.c $(CFLAGS) -o client

clean:
	rm -rf *.o
