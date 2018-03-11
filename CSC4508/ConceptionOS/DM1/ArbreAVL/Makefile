CC = gcc
RM = rm
CFLAGS = -Wall -Werror -g

all: arbreAVL

arbreAVL: arbreAVL.c
	${CC} ${CFLAGS} -o arbreAVL arbreAVL.c

clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s

cleanall: clean
	${RM} -f arbreAVL

