
CFLAGS=-Wall -Werror


all : libtab.so clean


libtab.so : tab.o
		gcc -shared -o libtab.so tab.o $(CFLAGS)

tab.o: tab.c tab.h
		gcc -o tab.o -c tab.c -I ../libtab -fPIC $(CFLAGS)


clean:
	rm -rf *.o
