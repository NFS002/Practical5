#include <stdio.h>
#include <stdlib.h>
#include "msg_queue.h"
#include "mq_safe_test.h"


/* Testing a threadsafe queue implementation. */
int main( void ) {

  unsigned const int n_thread = 100;

  // Create queue
  MQueue* queue = malloc(sizeof(MQueue));
  initMQueue(queue);

  // Test for deadlocks with 100 threads
	test_deadlocks_starvation(queue, n_thread);

  return 0;
}
