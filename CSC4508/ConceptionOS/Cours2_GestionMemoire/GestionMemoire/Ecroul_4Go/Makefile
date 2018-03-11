CC = gcc
RM = /bin/rm
CFLAGS = -Wall
BIN = alea use_memory cache_line 

all: $(BIN)

use_memory: use_memory.c outils.h
	${CC} ${CFLAGS} -o use_memory use_memory.c

cache_line: cache_line.c outils.h
	${CC} ${CFLAGS} -o cache_line cache_line.c

alea: alea.c outils.h
	${CC} ${CFLAGS} -o alea alea.c

clean:
	${RM} -f *~ *.bak *.tmp \
	         *.o *.s

cleanall: clean
	${RM} -f $(BIN)

