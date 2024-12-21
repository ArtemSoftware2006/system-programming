#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

pthread_cond_t is_value_changed;
pthread_mutex_t mutex;
int value = 0;
int changed = 0; 

void* observer(void* args) {
    while(1) {
        pthread_mutex_lock(&mutex);

        while (changed == 0) {
            pthread_cond_wait(&is_value_changed, &mutex);
        }

        printf("Value is changed: %d\n", value);
        changed = 0;
        sleep(1);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* changer(void* args) {
    int id = rand() % 10;
    while(1) {
        pthread_mutex_lock(&mutex);
        while (changed == 1) {
            pthread_mutex_unlock(&mutex);
            usleep(500000); // Пауза, чтобы другие потоки могли сработать
            pthread_mutex_lock(&mutex);
        }
        value = rand() % 100;
        printf("Changed by %d\n", id);
        changed = 1;
        pthread_cond_signal(&is_value_changed);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {

    pthread_cond_init(&is_value_changed, NULL);
    pthread_mutex_init(&mutex, NULL);

    int N;
    printf("Input number of threads:\n");
    scanf("%d", &N);
    N = N + 1;

    pthread_t threads[N];

    pthread_create(&threads[0], NULL, observer, NULL);
    for (int i = 1; i < N; i++)
    {
        pthread_create(&threads[i], NULL, changer, NULL);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&is_value_changed);

    return 0;
}