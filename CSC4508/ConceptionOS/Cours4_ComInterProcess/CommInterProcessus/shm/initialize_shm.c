/*************************/
/* initialize_shm.c */
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
#include <string.h>

/* key that identifies the shared memory */
#define SHM_KEY "/plop"

#define NB_ELEMENTS 10

typedef struct {
  int tab[NB_ELEMENTS];
} shared_memory;

shared_memory *buffer;

#define SHM_SIZE sizeof(shared_memory)

void do_start() {
  int fd, i;
  /* open the shared memory segment */
  fd = shm_open(SHM_KEY, O_CREAT| O_RDWR, 0666);
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

  /* initialize the shared memory buffer */
  for(i=0; i<NB_ELEMENTS; i++) {
    buffer->tab[i] = i;
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
}

void do_stop() {
  /* remove the shared memory segment */
  int retval = shm_unlink(SHM_KEY);
  /* shm_unlink may return ENOENT if the shared memory has already been unlinked
   * (by another process)
   */
  if(retval < 0 && errno != ENOENT){
    perror("shm_unlink");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2){
    fprintf(stderr, "USAGE: %s <start|stop>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if(!strcmp(argv[1], "start")) {
    do_start();
  } else if(!strcmp(argv[1], "stop")) {
    do_stop();
  } else {
    fprintf(stderr, "Unknown command '%s'\n", argv[1]);
  }

  return EXIT_SUCCESS;
}
