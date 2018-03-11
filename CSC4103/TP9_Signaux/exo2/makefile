
CFLAGS=-Wall -Werror


all : pong clean


pong : pong.o
		gcc -o pong pong.o $(CFLAGS)

pong.o: pong.c
		gcc -o pong.o -c pong.c -g $(CFLAGS)


clean:
	rm -rf *.o
