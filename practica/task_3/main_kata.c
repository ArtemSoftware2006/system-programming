#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUMBER 100

typedef struct {
    int thread_id;
    int* last_thread_id;
    pthread_mutex_t *mutex;
} ThreadData;

void* RunThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    int time_sleep = rand() % 1000;
    usleep(time_sleep * 1000);

    pthread_mutex_lock(data->mutex);
    *(data->last_thread_id) = data->thread_id;
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

int main () {

    pthread_t threads[THREAD_NUMBER];
    ThreadData threadDatas[THREAD_NUMBER];
    pthread_mutex_t mutex;
    int last_thread_id = -1;

    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        threadDatas[i].thread_id = i + 1;
        threadDatas[i].last_thread_id = &last_thread_id;
        threadDatas[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, RunThread, &threadDatas[i]);
    }

    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Last thread id: %d", last_thread_id);
    
    return 0;
}