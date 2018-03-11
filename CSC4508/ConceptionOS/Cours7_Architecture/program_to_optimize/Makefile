BIN= program_to_optimize
LDFLAGS=-lpthread
CFLAGS=-O0 -march=native

all: $(BIN)

program_to_optimize: program_to_optimize.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	rm -f $(BIN)

cleanall: clean
	${RM} -f $(BIN)

