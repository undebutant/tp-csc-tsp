
CFLAGS=-Wall -Werror -O3


all : hello clean


hello : hello.o
		gcc -o hello hello.o $(CFLAGS)

hello.o: hello.c
		gcc -o hello.o -c hello.c -g $(CFLAGS)


clean:
	rm -rf *.o
