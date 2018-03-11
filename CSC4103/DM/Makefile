
CFLAGS=-Wall -Werror


all : clean memory_alloc


memory_alloc: memory_alloc.c memory_alloc.h
		gcc -o memory_alloc memory_alloc.c -I ../memory_alloc.h $(CFLAGS)


clean:
	rm -rf memory_alloc
