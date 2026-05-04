#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include "queue.h"

#define NUM_THREADS 3

typedef struct {
    pthread_t threads[NUM_THREADS];
    Queue *queue;
    int stop;
} ThreadPool;

void init_thread_pool(ThreadPool *pool, Queue *queue);
void destroy_thread_pool(ThreadPool *pool);

#endif