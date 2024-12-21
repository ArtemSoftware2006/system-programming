#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t not_empty;
pthread_cond_t not_full;

void* producer(void* args) 
{
    int id = *((int*)args);
    free(args);

    while(1) 
    {
        int item = rand() % 100;
        usleep(500000);

        pthread_mutex_lock(&mutex);

        // Wait empty place in Buffer
        while (count == BUFFER_SIZE)
        {
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Producer %d produce %d\n", id, item);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* consumer(void* args) 
{
    int id = *((int*)args);
    free(args);

    while(1)
    {
        pthread_mutex_lock(&mutex);

        while (count == 0)
        {
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d consume %d\n", id, item);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        usleep(200000);
    }
    return NULL;
}

int main()
{
    int producers_number, consumers_number;

    printf("Введите количество производителей: ");
    scanf("%d", &producers_number);
    printf("Введите количество потребителей: ");
    scanf("%d", &consumers_number);

    pthread_t consumers[consumers_number];
    pthread_t producers[producers_number];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    for (int i = 0; i < producers_number; i++)
    {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&producers[i], NULL, producer, id);
    }

    for (int i = 0; i < consumers_number; i++)
    {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consumers[i], NULL, consumer, id);
    }

    for (int i = 0; i < producers_number; i++)
    {
        pthread_join(producers[i], NULL);
    }
    
    for (int i = 0; i < consumers_number; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}