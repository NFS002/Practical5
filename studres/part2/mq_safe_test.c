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
  for (int i = 0; i < 100; i++){
	 send_msg(arg, i) ;
 }
 return NULL;
}

int queue_length(MQueue* queue) {
  int n = 0;
  Node* tmp = queue->head;
  while (tmp != NULL) {
    n++;
    tmp = tmp->next;
  }
  return n;
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
void test_deadlocks_starvation_1(MQueue* queue, unsigned int n_thread) {

  /* Reset queue */
  initMQueue(queue);

  if (n_thread % 2 != 0 ) n_thread += 1;

  pthread_t threads[n_thread];

  for (unsigned int i = 1; i <= n_thread; i += 2) {

    pthread_create(&threads[i], NULL, push_100, queue);       /*  2. create thread */
                                                              /*     push_n        */
    pthread_create(&threads[i + 1], NULL, pop_50, queue);     /*  3. create thread */
                                                              /*     pop_n         */
  }

  /* Test starvation */

  int starvation = 0; // starvation is false initially

  for (unsigned int i = 1; i <= n_thread; i++)
    starvation = starvation ||
      (pthread_join(threads[i], NULL) != 0) ;                  /*  4. Wait for all child threads to complete
                                                                    If any threads are unable to complete,
                                                                    i.e starvation, pthread_join will not return
                                                                    0

                                                                  */
  if (starvation) puts("test_starvation_deadlocks_1: FAIL");
  else puts("test_starvation_deadlocks_1: PASS");
}

/* To test for race conditions there are several ways, including the test below:

1. push 100 from two threads simultaneously, and check length is 200

*/

void test_race_conditions_1(MQueue* queue) {

  /* Reset queue */
  initMQueue(queue);

  pthread_t threads[2];
  pthread_create(&threads[0], NULL, push_100, queue);
  pthread_join(threads[0], NULL);                                                            /*     push_n        */
  pthread_create(&threads[1], NULL, push_100, queue);
  pthread_join(threads[1], NULL);  


  if (queue_length(queue) == 200) puts("test_race_conditions_1: PASS");
  else puts("test_race_conditions_1: FAIL");

}
