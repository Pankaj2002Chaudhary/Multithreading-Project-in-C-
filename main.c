// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>
// #include "queue.h"

// Queue q;

// void* producer(void* arg) {
//     for(int i = 1; i <= 10; i++) {
//         enqueue(&q, i);
//         sleep(1);
//     }
//     return NULL;
// }

// void* consumer(void* arg) {
//     for(int i = 1; i <= 10; i++) {
//         dequeue(&q);
//         sleep(2);
//     }
//     return NULL;
// }

// int main() {
//     pthread_t prod, cons;

//     init_queue(&q);

//     pthread_create(&prod, NULL, producer, NULL);
//     pthread_create(&cons, NULL, consumer, NULL);

//     pthread_join(prod, NULL);
//     pthread_join(cons, NULL);

//     return 0;
// }
#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "thread_pool.h"

Queue q;
ThreadPool pool;

int main() {
    init_queue(&q);
    init_thread_pool(&pool, &q);

    // Add tasks
    for(int i = 1; i <= 10; i++) {
        enqueue(&q, i);
        sleep(1);
    }

    sleep(5); // allow workers to finish

    destroy_thread_pool(&pool);

    return 0;
}