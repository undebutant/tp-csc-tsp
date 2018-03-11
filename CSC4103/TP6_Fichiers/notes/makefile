
CFLAGS=-Wall -Werror


all : bdn modify clean new


bdn : bdn.o
		gcc -o bdn bdn.o $(CFLAGS)

bdn.o: bdn.c student.h
	gcc -c bdn.c student.h -g $(CFLAGS)

modify : modify.o
		gcc -o modify modify.o $(CFLAGS)

modify.o: modify.c student.h
	gcc -c modify.c student.h -g $(CFLAGS)

clean:
	rm -rf *.o *.gch output.txt

new:
	touch output.txt
