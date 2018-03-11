
CFLAGS=-Wall -Werror


all : bank clean


bank: bank.o
		gcc -o bank bank.o $(CFLAGS) -lpthread

bank.o: bank.c
		gcc -o bank.o -c bank.c -g $(CFLAGS)


clean:
	rm -rf *.o
