CC = gcc
CFLAGS = -g -Wall -std=c11 -D_XOPEN_SOURCE=700
CFLAGS_OLD = -g -Wall -std=c11

RM = rm -f

all: clean server client client2

client:
	$(CC) -o client client.c $(CFLAGS_OLD)

client2:
	$(CC) -o client2 client2.c $(CFLAGS)

server:
	$(CC) -o serv serv.c $(CFLAGS)

clean:
	-$(RM) serv client
