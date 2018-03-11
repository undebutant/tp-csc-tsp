
CFLAGS=-Wall -Werror

all: dependency main clean

dependency :
		make -C../libtab

main: foo.o main.o
	        gcc -o main main.o foo.o $(CFLAGS) -L../libtab -ltab

foo.o: foo.c foo.h
	        gcc -c foo.c $(CFLAGS)

main.o: main.c
	        gcc -c main.c -I../libtab $(CFLAGS)

clean:
	        rm -f main.o foo.o
