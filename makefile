CC = gcc

CFLAGS_OPTIMIZATION = -g
#CFLAGS_OPTIMIZATION = -O3
CFLAGS_VERSION = -std=c11
CFLAGS_WARNINGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wno-unused-result
CFLAGS_DEFINES = -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -D_POSIX_SOURCE=1 -D_DEFAULT_SOURCE=1 -D_GNU_SOURCE=1
CFLAGS = $(CFLAGS_OPTIMIZATION) $(CFLAGS_VERSION) $(CFLAGS_WARNINGS) $(CFLAGS_DEFINES)

CFLAGS_OLD = -g -Wall #for deprecated gethostbyname(3)

RM = rm -f

all: clean server client

client:
	$(CC) -o client client.c $(CFLAGS_OLD)

server:
	$(CC) -o serv serv.c $(CFLAGS)

clean:
	-$(RM) serv client
