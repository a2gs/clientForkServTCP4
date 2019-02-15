
CC = gcc
CFLAGS = -g -Wall -std=c11

RM = rm -f

all: clean server client

client:
	$(CC) -o client client.c $(CFLAGS)

server:
	$(CC) -o serv serv.c $(CFLAGS)

clean:
	-$(RM) serv client
