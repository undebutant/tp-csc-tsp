CC = gcc
RM = /bin/rm
CFLAGS = -Wall
all: pw pwWritev

pw: pw.c
	${CC} ${CFLAGS} -o pw pw.c

pwWritev: pwWritev.c
	${CC} ${CFLAGS} -o pwWritev pwWritev.c
clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s \
	         /tmp/ficTest*

cleanall: clean
	${RM} -f pw pwWritev

