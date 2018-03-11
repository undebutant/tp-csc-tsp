
CFLAGS=-Wall -Werror


all : client server clean


server : server.o
	gcc server.o -o server $(CFLAGS)

server.o: server.c
	gcc -c server.c -g $(CFLAGS)

client : client.o
	gcc client.o -o client $(CFLAGS)

client.o: client.c
	gcc -c client.c -g $(CFLAGS)

clean:
	rm -rf *.o
