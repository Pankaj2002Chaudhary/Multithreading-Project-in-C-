#ifndef QUEUE_H
#define QUEUE_H
#include <pthread.h>
#define MAX 5

typedef struct {
    int buffer[MAX];
    int front, rear;
    int count;

    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;

} Queue;

void init_queue(Queue *q);
void enqueue(Queue *q, int item);
int dequeue(Queue *q);

#endif