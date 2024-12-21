#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS_NUM 5

typedef struct {
    int* max_number;
    pthread_mutex_t *mutex;
} ThreadData;

void* ComputeMax(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    int number = rand() % 100;

    pthread_mutex_lock(data->mutex);
    if (*(data->max_number) < number)
    {
        *(data->max_number) = number;
    }
    pthread_mutex_unlock(data->mutex);

    printf("In function \nthread id = %d\nNumber:%d\n", pthread_self(), number);     
    fflush(stdout);

    return NULL;
}

int main() {    
    srand(time(NULL));

    pthread_t threads[THREADS_NUM];
    ThreadData threadDatas[THREADS_NUM];
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    int max_number = -1;

    for (int i = 0; i < THREADS_NUM; i++)
    {
        threadDatas[i].max_number = &max_number;
        threadDatas[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, ComputeMax, &threadDatas[i]);
    }

    for (int i = 0; i < THREADS_NUM; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("Max number: %d", max_number);

    return 0;
}