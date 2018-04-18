#include <pthread.h>

typedef struct {
  int n;
  pthread_mutex_t* mutex;
} Counter;


/* Push multiple items to the queue */
void * push_100(void * arg);

/* Pop multiple items from the queue */
void * pop_50(void * queue);

/* Test for deadlocks in the queue */
void test_deadlocks_starvation(MQueue* queue, unsigned int n_thread);

/* Initialize the counter */
void init_counter(Counter* counter);

/* Increment counter */
void inc_counter(Counter* counter);
