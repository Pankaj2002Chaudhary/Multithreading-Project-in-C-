#include <stdio.h>
#include <pthread.h>

// Thread function
void* myThread(void* arg) {
    int thread_num = *(int*)arg;
    printf("Thread %d is running\n", thread_num);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    int a = 1, b = 2, c = 3;

    // Create threads
    pthread_create(&t1, NULL, myThread, &a);
    pthread_create(&t2, NULL, myThread, &b);
    pthread_create(&t3, NULL, myThread, &c);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("All threads finished\n");

    return 0;
}