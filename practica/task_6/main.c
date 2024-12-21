#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define TARGET_VALUE 1000

typedef struct {
    pthread_mutex_t *mutex;
    int* value;
} ThreadData;


void* Increment(void* arg) {
    ThreadData* data = (ThreadData*) arg;

    while (1)
    {

        pthread_mutex_lock(data->mutex);
        if (*(data->value) >= TARGET_VALUE)
        {
            pthread_mutex_unlock(data->mutex);   
            return NULL;
        }
        
        *(data->value) += 1;
        pthread_mutex_unlock(data->mutex);   
        printf("%d\n", *(data->value));
    }

    return NULL;
}

#define THREAD_NUMBER 10

int main() {

    pthread_t threads[THREAD_NUMBER];
    ThreadData threadDatas[THREAD_NUMBER];
    pthread_mutex_t mutex;
    int value = 0;

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        threadDatas[i].mutex = &mutex;
        threadDatas[i].value = &value;
        pthread_create(&threads[i], NULL, Increment, &threadDatas[i]);
    }

    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Value: %d", value);
    pthread_mutex_destroy(&mutex);

    return 0;
}