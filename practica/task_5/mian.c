#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <limits.h>

typedef struct  {
    int* array;
    int start;
    int end;
    int* min_value;
    pthread_mutex_t* mutex;
} ThreadData;


void* ComputeMin(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    for (int i = data->start; i < data->end; i++)
    {
        printf("\nthread id = %d\nNumber:%d\n", pthread_self(), data->array[i]);     
        fflush(stdout);

        if (*(data->min_value) > data->array[i])
        {
            pthread_mutex_lock(data->mutex);
            *(data->min_value) = data->array[i];
            pthread_mutex_unlock(data->mutex);
        }
    }

    return NULL;
}

#define THREADS_NUMBER 10
#define ARRAY_SIZE 10

int main() {
    pthread_t threads[THREADS_NUMBER];
    ThreadData threadDatas[THREADS_NUMBER];
    int array[ARRAY_SIZE] = {10, 20, 30, 40, -23444, 60, 70, 80, 90, -123};
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    int min_value = INT_MAX;

    int chunk_size = ARRAY_SIZE / THREADS_NUMBER;

    for (int i = 0; i < THREADS_NUMBER; i++)
    {
        threadDatas[i].array = array;
        threadDatas[i].min_value = &min_value;
        threadDatas[i].mutex = &mutex;
        threadDatas[i].start = chunk_size * i;
        if (i == THREADS_NUMBER - 1)
        {
            threadDatas[i].end = ARRAY_SIZE;
        } else {
            threadDatas[i].end = chunk_size * (i + 1);
        }
        pthread_create(&threads[i], NULL, ComputeMin, &threadDatas[i]);
    }

    for (int i = 0; i < THREADS_NUMBER; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nResult\nMin value: %d", min_value);

    return 0;    
}