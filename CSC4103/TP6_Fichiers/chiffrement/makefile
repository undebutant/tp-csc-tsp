
CFLAGS=-Wall -Werror


all : chiffrementFile clean new


chiffrementFile : chiffrementFile.o
		gcc -o chiffrementFile chiffrementFile.o $(CFLAGS)

chiffrementFile.o: chiffrementFile.c
		gcc -o chiffrementFile.o -c chiffrementFile.c -g $(CFLAGS)


clean:
	rm -rf *.o EncryptedYolo.txt verif.txt

new:
	touch EncryptedYolo.txt verif.txt
