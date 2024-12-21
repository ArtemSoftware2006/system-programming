#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUMBER 3
#define MAX_VALUE 900
#define VALUE_COND_1 300
#define VALUE_COND_2 600
#define VALUE_COND_3 900

typedef struct {
    int thread_id;
    int* counter;
    pthread_mutex_t* mutex;
    pthread_barrier_t* barrier_300;
    pthread_barrier_t* barrier_600;
    pthread_barrier_t* barrier_900;
} ThreadData;

void* count(void* args) {
    ThreadData* data = (ThreadData*)args;    

    int* current_value = data->counter;
    while (*(current_value) < MAX_VALUE)
    {
        pthread_mutex_lock(data->mutex);

        *(current_value) += 1;

        pthread_mutex_unlock(data->mutex);

        printf("Current thread_%d count is %d\n", data->thread_id, *(data->counter));

        int cond_1, cond_2, cond_3 = 0;

        if (*(current_value) >= VALUE_COND_1 && *(current_value) < VALUE_COND_2 && cond_1 == 0)
        {
            printf("Thread %d lock in 300!\n", data->thread_id);
            pthread_barrier_wait(data->barrier_300);
            cond_1 = 1;
        }

        if (*(current_value) >= VALUE_COND_2 && *(current_value) < VALUE_COND_3 && cond_2 == 0)
        {
            printf("Thread %d lock in 600!\n", data->thread_id);
            pthread_barrier_wait(data->barrier_600);
            cond_2 = 1;
        }

        if (*(current_value) >= VALUE_COND_3 && cond_3 == 0)
        {
            printf("Thread %d lock in 900!\n", data->thread_id);
            pthread_barrier_wait(data->barrier_900);
            cond_3 = 1;
        }
        
    }

    return 0;
}

int main() {
    pthread_mutex_t mutex;    
    pthread_mutex_init(&mutex, NULL);

    pthread_barrier_t barrier_300, barrier_600, barrier_900;
    pthread_barrier_init(&barrier_300, NULL, THREAD_NUMBER);
    pthread_barrier_init(&barrier_600, NULL, THREAD_NUMBER);
    pthread_barrier_init(&barrier_900, NULL, THREAD_NUMBER);

    pthread_t threads[THREAD_NUMBER];
    ThreadData datas[THREAD_NUMBER];

    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        datas[i].thread_id = i + 1;
        datas[i].counter = (int*)malloc(sizeof(int));
        *(datas[i].counter) = 0;
        datas[i].mutex = &mutex;
        datas[i].barrier_300 = &barrier_300;
        datas[i].barrier_600 = &barrier_600;
        datas[i].barrier_900 = &barrier_900;

        pthread_create(&threads[i], NULL, count, &datas[i]);
    }   

    for(int i = 0; i < THREAD_NUMBER; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier_300);
    pthread_barrier_destroy(&barrier_600);
    pthread_barrier_destroy(&barrier_900);

    printf("End");
    return 0;
}