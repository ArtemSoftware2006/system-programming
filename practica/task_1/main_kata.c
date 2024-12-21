#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int turn = 0;

void* PrintFoo(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while (turn != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Foo");
        fflush(stdout);
        turn = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

void* PrintBar(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while(turn != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Bar");
        fflush(stdout);
        turn = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread_1, thread_2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&thread_1, NULL, PrintFoo, NULL);
    pthread_create(&thread_2, NULL, PrintBar, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}