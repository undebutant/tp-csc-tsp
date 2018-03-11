CC = gcc
RM = rm
CFLAGS = -Wall -g
LDFLAGS = -g -lrt -pthread

all: client serveur

client: client.c client-serveur.h
	${CC} ${CFLAGS} -o client client.c ${LDFLAGS}

serveur: serveur.c client-serveur.h
	${CC} ${CFLAGS} -o serveur serveur.c ${LDFLAGS}

clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s

cleanall: clean
	${RM} -f client serveur client-serveur

