/**
@file msg_queue.c
@desc contains all function definitions and global variable for
implementing a thread safe message queue
*/
#include <stdio.h>
#include <stdlib.h>
#include "msg_queue.h"


/* *
Function definition: initMqueue.
@brief initializes the message queue by setting both the gead and the tail to null.
*/
void initMQueue(MQueue* queue) {
  queue->head = queue->tail = NULL;
  queue->mutex = malloc(sizeof(pthread_mutex_t));
}

/* *
Function definition: send_msg.
@brief enqueue a message with the given the value onto the back (tail) of the message queue.
@param queue a pointer to the MQueue, to which the message should be enqueued
@param value the value of the message
*/
void send_msg(MQueue* queue, int value) {
  pthread_mutex_lock(queue->mutex);
  Node* node = (Node*) malloc(sizeof(Node));
  node->data = value;
  node->next = NULL;
  if (queue->head && queue->tail) {
    queue->tail->next = node;
    queue->tail = node;
  }
  else queue->tail = queue->head = node;
  pthread_mutex_unlock(queue->mutex);
}

/* *
Function definition: read_msg.
@brief dequeue a message from the head message queue.
@param queue a pointer to the MQueue, from which the message should be dequeued
*/
Node* read_msg(MQueue* queue) {
  pthread_mutex_lock(queue->mutex);
  Node* node = queue->head;
  if (node) queue->head = node->next;
  pthread_mutex_unlock(queue->mutex);
  return node;
}

/* *
Function definition: print_MQueue.
@brief Prints the message queue
@param queue the message queue to be printed
*/
void printMQueue(MQueue* queue) {
  pthread_mutex_lock(queue->mutex);
  Node* tmp = queue->head;
  printf("[");
  while (tmp) {
    printf("%d",tmp->data);
    if (tmp->next) printf(",");
    tmp = tmp->next;
  }
  printf("]\n");
  pthread_mutex_unlock(queue->mutex);
}
