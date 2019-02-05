all: clean server client

client:
	cc -o client client.c -Wall -std=c11

server:
	cc -o serv serv.c -Wall -std=c11

clean:
	-rm -rf serv client
