CC = gcc
RM = /bin/rm
CFLAGS = -Wall

all: calcul disque

calcul: calcul.c
	${CC} ${CFLAGS} -o calcul calcul.c -lm

disque: disque.c
	${CC} ${CFLAGS} -o disque disque.c

clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s

cleanall: clean
	${RM} -f calcul disque


