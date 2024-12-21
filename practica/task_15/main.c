#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ARRAY_LENGTH 1000
#define THREAD_LENGTH 100

typedef struct {
    pthread_mutex_t* mutex;
    int* sum;
    int start;
    int end;
    int* array;
} ThreadData;

void* CalkSum(void* args) {
    ThreadData* data = (ThreadData*)args;

    int partitial_sum = 0;
    for (int i = data->start; i < data->end; i++)
    {
        partitial_sum += data->array[i];
    }

    pthread_mutex_lock(data->mutex);
    *(data->sum) += partitial_sum;
    printf("Thread resul = %d\n", partitial_sum);
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

int main() 
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    int* sum = malloc(sizeof(int));

    int array[ARRAY_LENGTH];

    for (int i = 0; i < ARRAY_LENGTH; i++)
    {
        array[i] = rand() % 100;
    }

    int thread_count = ceil(ARRAY_LENGTH / THREAD_LENGTH);
    pthread_t threads[thread_count];
    ThreadData datas[thread_count];
    for (int i = 0; i < thread_count; i++)
    {
        datas[i].array = array;
        datas[i].mutex = &mutex;
        datas[i].sum = sum;
        datas[i].start = THREAD_LENGTH * i;
        datas[i].end = THREAD_LENGTH * (i + 1);

        pthread_create(&threads[i], NULL, CalkSum, &datas[i]);
    }

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nThreads result = %d\n", *sum);

    *sum = 0;
    for (int i = 0; i < ARRAY_LENGTH; i++)
    {
        *sum += array[i];
    }

    printf("\nLoop result = %d", *sum);
    
    return 0;
}