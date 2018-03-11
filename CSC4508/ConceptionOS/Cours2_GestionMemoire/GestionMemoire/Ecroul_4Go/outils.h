#define _GNU_SOURCE
#include <sched.h>

static int bind_to_core(int corenum)
{
  /* cpu_set function are not available on Mac OSX :( */
#ifndef __APPLE__
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(corenum, &cpuset);

  int ret = sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
  if(ret) {
    perror("bind_to_core");
  }
#endif
  return ret;
}

static char** allocate_memory(size_t nbre, int page)
{
  int i;
  printf("Allocating %lu * %d bytes\n", nbre, page);
  char **t = malloc(nbre * sizeof(char*));
  if(!t) {
    perror("allocate_memory");
    return NULL;
  }

  for(i=0; i<nbre; i++) {
    t[i] = malloc(sizeof(char) * page);
    t[i][0] = 0; // pour etre sur que le buffer est bien alloue.
  }
  return t;
}
