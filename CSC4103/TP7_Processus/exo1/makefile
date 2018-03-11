
CFLAGS=-Wall -Werror


all : fork_chain clean


chiffrementFile : fork_chain.o
		gcc -o fork_chain fork_chain.o $(CFLAGS)

chiffrementFile.o: fork_chain.c
		gcc -o fork_chain.o -c fork_chain.c -g $(CFLAGS)


clean:
	rm -rf *.o
