/*************************/
/* pingpong_shm.c */
/*************************/

/* Perform a pingpong between 2 processes using shared memory.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
#include <time.h>

/* Key for the shared memory */
#define SHM_KEY "/plop"

#define NB_ITER 10000000
#define WARMUP 100

#define NB_ELEMENTS 1

#define SHM_SIZE (2*(sizeof(int) * (NB_ELEMENTS + 1)))


/* The shared memory region is composed of of:
 * - two mailboxes
 * - two buffers
 *
 * The shared memory region looks like:
 * -------------------------------------------------------------------------
 * | mailbox[0] | mailbox[1] |        buffer[0]     |       buffer[1]      |
 * -------------------------------------------------------------------------
 *
 * Sending a message to process i consists in copying the message to buffer[i]
 * and then setting mailbox[i] to 1.
 */

void write_to_buffer(int* buffer, int size, int value) {
  int i;

  for(i=0; i<size; i++) {
    buffer[i] = value;
  }
}

void print_buffer(int* buffer, int size) {
  int i;

  for(i=0; i<size; i++) {
    printf("%d ", buffer[i]);
  }
  printf("\n");
}

void pingpong(int nb_iter, int rank, volatile int** mailbox, int**buffer)
{
  int i;
  if(rank == 0) {

    for(i=0; i<nb_iter; i++) {
      /* send a message to process 1 */
      write_to_buffer(buffer[1], NB_ELEMENTS, i);
      *mailbox[1] = 1;

      /* receive a message */
      while(*mailbox[0] != 1)  { }
      *mailbox[0] = 0;

    }
  } else {

    for(i=0;i<nb_iter; i++) {
      /* receive a message */
      while(*mailbox[1] != 1)  { }
      *mailbox[1] = 0;

      /* send a message to process 0 */
      write_to_buffer(buffer[0], NB_ELEMENTS, i);
      *mailbox[0] = 1;
    }
  }
}

int main(int argc, char *argv[]) {
  int fd;

  int* shared_memory = NULL;

  int *buffer[2];
  volatile int *mailbox[2];

  int rank;

  if (argc < 2){
    fputs( "USAGE = exemple_shm <0|1>\n", stderr);
    exit(EXIT_FAILURE);
  }

  rank = atoi(argv[1]);

  /* open the shared memory segment */
  fd = shm_open(SHM_KEY, O_CREAT|O_RDWR, 0666);
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
  if((shared_memory = mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) <0) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  mailbox[0] = (volatile int*)shared_memory;
  mailbox[1] = (volatile int*)shared_memory + sizeof(int);

  buffer[0] = (int*)shared_memory + (sizeof(int)*2);
  buffer[1] = (int*)shared_memory + (sizeof(int)*(2+NB_ELEMENTS));


  if(!rank) {
    /* warmup before measurement */
    pingpong(WARMUP, 0, mailbox, buffer);

    /* measurement */
    pingpong(NB_ITER, 0, mailbox, buffer);
  } else {

    /* warmup before measurement */
    pingpong(WARMUP, 1, mailbox, buffer);

    /* measurement */
    struct timespec t1, t2;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t1);
    pingpong(NB_ITER, 1, mailbox, buffer);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t2);

    double duration = ((t2.tv_sec - t1.tv_sec)*1e9 + (t2.tv_nsec - t1.tv_nsec))/1e9;
    printf("%d loops in %lf s -> %lf ns per loop\n", NB_ITER, duration, (duration/NB_ITER)*1e9);

  }

  /* unmap the memory region */
  if(munmap(shared_memory, SHM_SIZE) < 0) {
    perror("munmap");
    exit(EXIT_FAILURE);
  }

  /* close the shared memory segment */
  if(close(fd) < 0){
    perror("close");
    exit(EXIT_FAILURE);
  }

  /* remove the shared memory segment */
  int retval = shm_unlink(SHM_KEY);
  /* shm_unlink may return ENOENT if the shared memory has already been unlinked
   * (by another process)
   */
  if(retval < 0 && errno != ENOENT){
    perror("shm_unlink");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
