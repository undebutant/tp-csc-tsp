CC = gcc
RM = /bin/rm
CFLAGS = -Wall -g -pg
LDFLAGS = -pg

EXE = bug cmpAlloc

all: ${EXE}

bug:  bug.o monAllocateur.o
cmpAlloc: cmpAlloc.o monAllocateur.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s

cleanall: clean
	${RM} -f ${EXE} *.out


