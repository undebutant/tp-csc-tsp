#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdint.h>
#include <immintrin.h>		/* for SSE/AVX intrinsics */

#define NB_TH 4
#define NB_JOBS 500000000
#define NB_EVENTS (NB_JOBS/100)

struct event {
  int result;
};

struct trace {
  int nb_events;
  struct event *event_array;
};

struct trace t_th[NB_TH];
pthread_barrier_t thread_ready;
pthread_barrier_t start_processing;

#define TIME_DIFF(t1, t2) (((t2).tv_sec-(t1).tv_sec)*1e6+((t2).tv_usec-(t1).tv_usec))

void analyze_events(int thread_id) {
  int i;
  int result = 0;

  for(i=0; i<NB_EVENTS; i++) {
    result += t_th[thread_id].event_array[i].result;
  }

  int average_result = result/NB_EVENTS;
  printf("average result for thread %d: %d\n", thread_id, average_result);
}

void* thread_function(void* arg) {
  int my_id= *(int*)arg;
  int i;
  t_th[my_id].nb_events = 0;
  t_th[my_id].event_array = malloc(sizeof(struct event)*NB_EVENTS);

  /* tell the main thread that we are ready */
  pthread_barrier_wait(&thread_ready);

  /* wait until all the threads are started */
  pthread_barrier_wait(&start_processing);
  printf("Thread %d starts processing jobs\n", my_id);

  for(i=0; i<NB_JOBS; i++) {
    int nb_events = t_th[my_id].nb_events++;
    t_th[my_id].event_array[nb_events%NB_EVENTS].result = i;
  }
  printf("Thread %d finished processing jobs\n", my_id);

  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
  analyze_events(my_id);
  gettimeofday(&t2, NULL);

  double duration = TIME_DIFF(t1, t2)/1e3;
  printf("analysis took %lf ms\n", duration);
  return NULL;
}

int main() {
  int i;
  pthread_barrier_init(&thread_ready, NULL, 2);
  pthread_barrier_init(&start_processing, NULL, NB_TH+1);
  pthread_t tids[NB_TH];

  for(i=0; i<NB_TH; i++) {
    pthread_create(&tids[i], NULL, thread_function, &i);
    pthread_barrier_wait(&thread_ready);
  }
  /* all the threads are created and are waiting to start processing */
  pthread_barrier_wait(&start_processing);

  for(i=0; i<NB_TH; i++) {
    pthread_join(tids[i], NULL);
  }
  return 0;
}
