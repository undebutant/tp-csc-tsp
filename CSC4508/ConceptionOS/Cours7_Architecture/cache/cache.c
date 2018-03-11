#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>

/* This program requires HWLoc.
 * On most systems, you need to install the libhwloc-dev or hwloc-devel package
 */

#define NB_TH 2
#define NB_ITER 100000000

#define GET_TIME(t) gettimeofday(&(t), NULL)
#define TIME_DIFF(t1, t2) (((t2).tv_sec - (t1).tv_sec)*1e6 + ((t2).tv_usec - (t1).tv_usec))

pthread_barrier_t barrier;

#if HAVE_HWLOC
#include <hwloc.h>
hwloc_topology_t topology;
#endif


/* bind a thread on core #core_id */
void bind_thread(unsigned core_id) {
#if HAVE_HWLOC
  /* find the number of processing units (core, PUs, etc.) */
  int depth = hwloc_topology_get_depth(topology);
  hwloc_obj_type_t type = hwloc_get_depth_type(topology, depth-1);

  /* get the core_id th processing unit */
  hwloc_obj_t obj = hwloc_get_obj_by_type (topology, type, core_id);

  /* bind the current thread to this processing unit */
  printf("Binding thread to PU %d\n", obj->logical_index);
  hwloc_set_cpubind(topology, obj->cpuset, HWLOC_CPUBIND_THREAD);
#else
  printf("Binding not implemented!\n");
#endif
}

void *thread_func(void*arg) {
  int* counter = (int*)arg;
  bind_thread(*counter);
  int ret;
  ret = pthread_barrier_wait(&barrier);
  assert(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);
  int i;
  for(i=0; i<NB_ITER; i++) {
    (*counter) = i;
  }

  ret = pthread_barrier_wait(&barrier);
  assert(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);

  printf("result[%p]=%d\n", counter, *counter);
  return NULL;
}

struct counter_struct{
  int value;
};

int main(int argc, char**argv) {
  int ret;
  int i;
  pthread_t tids[NB_TH];
#if HAVE_HWLOC
  hwloc_topology_init(&topology);  // initialize hwloc
  hwloc_topology_load(topology);   // detect the topology
#endif

  struct counter_struct counters[NB_TH];
  struct timeval t1, t2;

  if(argc < NB_TH+1) {
    printf("Usage: %s cpu1 cpu2 ...\n", argv[0]);
    return EXIT_FAILURE;
  }

  ret = pthread_barrier_init(&barrier, NULL, NB_TH+1);
  assert(ret==0);

  for(i=0; i<NB_TH; i++) {
    counters[i].value = atoi(argv[i+1]);
    ret = pthread_create(&tids[i], NULL, thread_func, &counters[i].value);
    assert(ret == 0);
  }

  /* Start measurement */
  ret = pthread_barrier_wait(&barrier);
  assert(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);
  GET_TIME(t1);

  /* Stop measurement */
  ret = pthread_barrier_wait(&barrier);
  assert(ret == 0 || ret == PTHREAD_BARRIER_SERIAL_THREAD);
  GET_TIME(t2);

  for(i=0; i<NB_TH; i++) {
    ret = pthread_join(tids[i], NULL);
    assert(ret == 0);
  }

  double duration = TIME_DIFF(t1, t2)/1000;
  printf("%d iterations in %lf ms\n", NB_ITER, duration);

#if HAVE_HWLOC
  hwloc_topology_destroy(topology);
#endif
  return EXIT_SUCCESS;
}
