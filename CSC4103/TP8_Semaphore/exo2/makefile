
CFLAGS=-Wall -Werror


all : sem clean


sem: sem.o
		gcc -o sem sem.o $(CFLAGS) -lpthread

sem.o: sem.c
		gcc -o sem.o -c sem.c -g $(CFLAGS)


clean:
	rm -rf *.o
