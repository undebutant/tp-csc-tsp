########################################################################
# Code listing from "Advanced Linux Programming," by CodeSourcery LLC  #
# Copyright (C) 2001 by New Riders Publishing                          #
# See COPYRIGHT for license information.                               #
########################################################################

### Configuration.  ####################################################

# C source files for the server.
SOURCES		= common.c codeEtudiant.c server.c main.c
# Object files corresponding to source files.
OBJECTS		= $(SOURCES:.c=.o)
# Compilation flags
CFLAGS = -Wall -pthread

### Rules.  ############################################################

.PHONY:         all clean

# Default target: build everything.
all:            server $(MODULES)

# Clean up build products.
clean:
	rm -f $(OBJECTS) $(MODULES) server *~

cleanall: clean

# The main server program.  Link with -Wl,-export-dyanamic so
# dynamically loaded modules can bind symbols in the program.  Link in
# libdl, which contains calls for dynamic loading.
server:         $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# All object files in the server depend on server.h.  But use the
# default rule for building object files from source files.
$(OBJECTS):     server.h codeEtudiant.h
