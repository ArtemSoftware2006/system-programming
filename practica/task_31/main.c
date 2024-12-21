#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define STOP_CONDITION 5000

int counter_1, counter_2, counter_3 = 0;
int stop_cond = 0;
pthread_mutex_t mutex;

void* thread_1_func(void* args) {
    while(1) {
        pthread_mutex_lock(&mutex);
        if (counter_1 * counter_3 >= STOP_CONDITION)
        {
            stop_cond = 1;
        }
        if (stop_cond == 1)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        counter_1++;
        printf("Thread 1: %d\n", counter_1);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }

    return NULL;
}

void* thread_2_func(void* args) {
    while(1) {
        pthread_mutex_lock(&mutex);
        if (stop_cond == 1) {
            printf("\n\nTHREAD 2 IS STOPPED!!!\n\n");
            printf("counter_1 : %d\ncounter_3 : %d\n\n", counter_1, counter_3);
            pthread_mutex_unlock(&mutex);
            break;
        }
        counter_2++;
        printf("Thread 2: %d\n", counter_2);
        pthread_mutex_unlock(&mutex);
        usleep(150000);
    }

    return NULL;
}

void* thread_3_func(void* args) {
    while(1) {
        pthread_mutex_lock(&mutex);
        if (counter_1 * counter_3 >= STOP_CONDITION)
        {
            stop_cond = 1;
        }
        if (stop_cond == 1)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        counter_3++;
        printf("Thread 3: %d\n", counter_3);
        pthread_mutex_unlock(&mutex);
        usleep(170000);
    }

    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_t thread_1, thread_2, thread_3;

    pthread_create(&thread_1, NULL, thread_1_func, NULL);
    pthread_create(&thread_2, NULL, thread_2_func, NULL);
    pthread_create(&thread_3, NULL, thread_3_func, NULL);
    
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}