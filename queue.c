#include <stdio.h>
#include <pthread.h>
#include "queue.h"

void init_queue(Queue *q) {
    q->front = q->rear = q->count = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

void enqueue(Queue *q, int item) {  //Producer
    pthread_mutex_lock(&q->lock);

    while(q->count == MAX) {
        pthread_cond_wait(&q->not_full, &q->lock);
    }

    q->buffer[q->rear] = item;
    q->rear = (q->rear + 1) % MAX;
    q->count++;

    printf("Produced: %d\n", item);

    pthread_cond_signal(&q->not_empty); //Wake up consumer (now queue has data)
    pthread_mutex_unlock(&q->lock); //Release lock
}

int dequeue(Queue *q) {
    pthread_mutex_lock(&q->lock);

    while(q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }

    int item = q->buffer[q->front];
    q->front = (q->front + 1) % MAX;
    q->count--;

    printf("Consumed: %d\n", item);

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);

    return item;
}