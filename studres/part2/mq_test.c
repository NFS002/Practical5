#include <stdio.h>
#include <stdlib.h>
#include "msg_queue.h"

int main() {

  // Create queue
  MQueue* queue = malloc(sizeof(MQueue));
  initMQueue(queue);

  // Put stuff into queue
  for (int i = 0; i <= 100; i++) {
	 send_msg(queue, i);
  }

  // Print the queue
  printMQueue(queue);

  return 0;
}
