all: clean server client

client:
	cc -o client client.c

server:
	cc -o serv serv.c

clean:
	-rm -rf serv client
