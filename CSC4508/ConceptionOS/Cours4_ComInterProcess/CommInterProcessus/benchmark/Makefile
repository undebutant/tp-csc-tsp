### Configuration.  ####################################################

# targets
BIN = tube_4096 tube_416 tube_malloc tableau_semaphore cond file_messages file_messages_posix

# Additional source file for each target
SOURCES		= simulateur.c

# Object files corresponding to source files.
OBJECTS		= $(SOURCES:.c=.o)

# Compilation flags
CFLAGS = -Wall -pthread -O3
LDFLAGS = -lrt

### Rules.  ############################################################

.PHONY:         all clean

# Default target: build everything.
all:            $(BIN)

# Clean up build products.
clean:
	rm -f $(OBJECTS) serveur *~ $(BIN)

cleanall: clean

%: %.c $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# default rule for building object files from source files.
$(OBJECTS):     simulateur.h
