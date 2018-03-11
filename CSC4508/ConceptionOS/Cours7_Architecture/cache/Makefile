BIN=cache_line cache cache_associativity
CFLAGS=-O0 -g -Wall $(shell pkg-config --cflags hwloc) -DHAVE_HWLOC=1
LDFLAGS=-lpthread $(shell pkg-config --libs hwloc)


all: $(BIN)

clean:
	rm -f $(BIN)

cleanall: clean
	${RM} -f $(BIN)

