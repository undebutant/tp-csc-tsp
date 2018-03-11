CC = gcc
RM = /bin/rm
CFLAGS = -Wall -Werror
all: readIndex

readIndex: readIndex.c
	${CC} ${CFLAGS} -o readIndex readIndex.c

clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s \

cleanall: clean
	${RM} -f readIndex

