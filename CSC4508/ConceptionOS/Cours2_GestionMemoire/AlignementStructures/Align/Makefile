CC = gcc
RM = rm
CFLAGS = -Wall -O3

all: align nonAlign nonAlignPacked testAlign

align: useGetrusage.o align.o constantes.h useGetrusage.h
	${CC} ${CFLAGS} -o align useGetrusage.o align.o
	# On genere l'assembleur pour comparer le code genere
	${CC} ${CFLAGS} -S align.c

nonAlign: useGetrusage.o nonAlign.o constantes.h useGetrusage.h
	${CC} ${CFLAGS} -o nonAlign useGetrusage.o nonAlign.o
	# On genere l'assembleur pour comparer le code genere
	${CC} ${CFLAGS} -S nonAlign.c

nonAlignPacked: useGetrusage.o nonAlignPacked.o constantes.h useGetrusage.h
	${CC} ${CFLAGS} -o nonAlignPacked useGetrusage.o nonAlignPacked.o
	# On genere l'assembleur pour comparer le code genere
	${CC} ${CFLAGS} -S nonAlignPacked.c

testAlign: testAlign.c constantes.h useGetrusage.h Makefile
	${CC} ${CFLAGS} -DALIGN -o testAlign useGetrusage.o testAlign.c
#	${CC} ${CFLAGS} -o testAlign useGetrusage.o testAlign.c

clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s

cleanall: clean
	${RM} -f align nonAlign nonAlignPacked testAlign

