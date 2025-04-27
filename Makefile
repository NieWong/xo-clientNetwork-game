CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lpthread

all: server client

server: server.o csapp.o
	$(CC) $(CFLAGS) -o server server.o csapp.o $(LDFLAGS)

client: client.o csapp.o
	$(CC) $(CFLAGS) -o client client.o csapp.o $(LDFLAGS)

server.o: server.c csapp.h tictac.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c csapp.h tictac.h
	$(CC) $(CFLAGS) -c client.c

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

clean:
	rm -f server client *.o *~

.PHONY: all clean