CC = gcc
CFLAGS = -g -Wall -std=c11 -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -D_POSIX_SOURCE=1 -D_DEFAULT_SOURCE=1 -D_GNU_SOURCE=1
#CFLAGS = -O2 -Wall -std=c11 -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -D_POSIX_SOURCE=1 -D_DEFAULT_SOURCE=1 -D_GNU_SOURCE=1
CFLAGS_OLD = -g -Wall #for deprecated gethostbyname(3)

RM = rm -f

all: clean server client

client:
	$(CC) -o client client.c $(CFLAGS_OLD)

server:
	$(CC) -o serv serv.c $(CFLAGS)

clean:
	-$(RM) serv client
