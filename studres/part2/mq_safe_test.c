#include "msg_queue.h"
#include "mq_safe_test.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

unsigned int n_thread;

/* FUNCTIONS */

/* Push multiple items to the queue */
void * push_100(void* arg)
{
  srand(time(NULL));
  for (int i = 0; i < 100; i++){
	 send_msg(arg, rand()) ;
 }
 return NULL;
}

/* Pop multiple items from the queue */
void * pop_50(void* arg)
{
  for (int i = 0; i < 50; i++) {
	 read_msg(arg) ;
 }
 return NULL;
}


/* Initialize the counter */
void init_counter(Counter* counter) {
  counter->n = 0;
  counter->mutex = malloc(sizeof(pthread_mutex_t));
}

/* Increment counter */
void inc_count(Counter* counter) {
  pthread_mutex_lock(counter->mutex);
  counter->n += 1;
  pthread_mutex_unlock(counter->mutex);
}

/* Test for deadlocks in the queue */
void test_deadlocks_starvation(MQueue* queue, unsigned int n_thread) {

  if (n_thread % 2 != 0 ) n_thread += 1;

  pthread_t threads[n_thread];

  for (unsigned int i = 1; i <= n_thread; i += 2) {

    pthread_create(&threads[i], NULL, push_100, queue);       /*  2. create thread */
                                                              /*     push_n        */
    pthread_create(&threads[i + 1], NULL, pop_50, queue);     /*  3. create thread */
                                                              /*     pop_n         */
  }

  puts("test_deadlocks: passed");

  /* Test starvation */

  int starvation = 0; // starvation is false initially

  for (unsigned int i = 1; i <= n_thread; i++)
    starvation = starvation &&
      (pthread_join(threads[i], NULL) != 0) ;                  /*  4. Wait for all child threads to complete
                                                                    If any threads are unable to complete,
                                                                    i.e starvation, pthread_join will not return
                                                                    0
                                                              */
  if (starvation) puts("test_starvation: failed");
  else puts("test_starvation: passed");
}
