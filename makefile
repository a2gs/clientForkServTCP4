all: clean server client

client:
	cc -o client client.c -Wall

server:
	cc -o serv serv.c -Wall

clean:
	-rm -rf serv client
