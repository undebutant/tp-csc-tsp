
CFLAGS=-Wall -Werror


all : memory clean


memory : memory.o
		gcc -o memory memory.o $(CFLAGS)

memory.o: memory.c
		gcc -o memory.o -c memory.c -g $(CFLAGS)


clean:
	rm -rf *.o
