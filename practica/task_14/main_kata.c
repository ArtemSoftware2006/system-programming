#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


int sum = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* input(void* args) {
    int num;
    while(1) {
        printf("\rInpout number: ");
        fflush(stdout);
        if(scanf("%d", &num) != 1) {
            printf("ERROR: Incorect input!");
            exit(0);
        }

        pthread_mutex_lock(&mutex);
        sum += num;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void* output(void* args) {
    while(1) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("\r Sum: %d", sum);
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int main() {
    pthread_t thread_out, thread_in;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&thread_in, NULL, input, NULL);
    pthread_create(&thread_out, NULL, output, NULL);

    pthread_join(thread_in, NULL);
    pthread_join(thread_out, NULL);

    return 0;
}