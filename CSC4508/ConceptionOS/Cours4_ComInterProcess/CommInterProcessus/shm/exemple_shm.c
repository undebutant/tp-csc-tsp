/*************************/
/* exemple_shm.c */
/*************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <stdint.h>

/* key that identifies the shared memory */
#define SHM_KEY "/plop"

#define NB_ELEMENTS 10

typedef struct {
  int tab[NB_ELEMENTS];
} shared_memory;

shared_memory *buffer;

#define SHM_SIZE sizeof(shared_memory)

void print_buffer() {
  int i;
  printf("The shared memory segment contains:\n");
  for(i=0; i<NB_ELEMENTS; i++) {
    printf("%d\t", buffer->tab[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  int fd;
  int rank;
  int i;

  if (argc < 2){
    fputs( "USAGE: exemple_shm <rank>\n", stderr);
    exit(EXIT_FAILURE);
  }

  rank = atoi(argv[1]);

  /* open the shared memory segment */
  fd = shm_open(SHM_KEY, O_RDWR, 0666);
  if(fd<0) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  /* set the size of the shared memory segment */
  if(ftruncate(fd, SHM_SIZE) < 0) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  /* map the shared memory segment in the current address space */
  if((buffer = mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) <0) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }



  for(i=0; i<10; i++) {
    print_buffer();
    int v = buffer->tab[rank];
    sleep(1);
    v++;
    buffer->tab[rank] = v;
  }


  /* unmap the memory region */
  if(munmap(buffer, SHM_SIZE) < 0) {
    perror("munmap");
    exit(EXIT_FAILURE);
  }

  /* close the shared memory segment */
  if(close(fd) < 0){
    perror("close");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
