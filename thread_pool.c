#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "thread_pool.h"

// Worker thread function → acts as CONSUMER
void* worker(void* arg) {

    // Cast argument back to ThreadPool pointer
    ThreadPool *pool = (ThreadPool*)arg;

    // Infinite loop → thread keeps processing tasks
    while(1) {

        // Lock the queue before accessing shared data
        pthread_mutex_lock(&pool->queue->lock);

        // Wait if queue is empty AND stop flag is not set
        // Releases lock and sleeps until signaled
        while(pool->queue->count == 0 && !pool->stop) {
            pthread_cond_wait(&pool->queue->not_empty, &pool->queue->lock);
        }

        // If stop signal is given → exit thread
        if(pool->stop) {
            pthread_mutex_unlock(&pool->queue->lock); // unlock before exiting
            break;
        }

        //  Remove task from queue (CRITICAL SECTION)
        int task = pool->queue->buffer[pool->queue->front];

        // Circular queue update
        pool->queue->front = (pool->queue->front + 1) % MAX;
        pool->queue->count--;

        //  Signal producer that space is available
        pthread_cond_signal(&pool->queue->not_full);

        //  Unlock queue after modification
        pthread_mutex_unlock(&pool->queue->lock);

        // Process task (outside critical section → better performance)
        printf("Thread %lu processing task %d\n", pthread_self(), task);

        // Simulate work
        sleep(1);
    }

    return NULL;
}


// Initialize thread pool → create worker threads
void init_thread_pool(ThreadPool *pool, Queue *queue) {

    // Assign shared queue to pool
    pool->queue = queue;

    // stop = 0 → threads should keep running
    pool->stop = 0;

    // Create fixed number of worker threads
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&pool->threads[i], NULL, worker, pool);
    }
}


// Destroy thread pool → graceful shutdown
void destroy_thread_pool(ThreadPool *pool) {

    // Lock queue to safely update stop flag
    pthread_mutex_lock(&pool->queue->lock);

    // Signal all threads to stop
    pool->stop = 1;

    // Wake up ALL waiting threads (important)
    pthread_cond_broadcast(&pool->queue->not_empty);

    //  Unlock queue
    pthread_mutex_unlock(&pool->queue->lock);

    //  Wait for all worker threads to finish
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(pool->threads[i], NULL);
    }
}