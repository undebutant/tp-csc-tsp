
CFLAGS=-Wall -Werror


all : main clean


main : main.o
		gcc -o executable main.o $(CFLAGS)

main.o: main04.c
		gcc -o main.o -c main04.c -g $(CFLAGS)

clean:
	rm -f *.o

