#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int turn = 1; // 0 для Foo, 1 для Bar

void *printFoo(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (turn != 0) { // Ждём своей очереди
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Foo");
        fflush(stdout);
        turn = 1; // Передаём очередь второму потоку
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *printBar(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (turn != 1) { // Ждём своей очереди
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Bar");
        usleep(1000 * 100);
        fflush(stdout);
        turn = 0; // Передаём очередь первому потоку
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&thread1, NULL, printFoo, NULL);
    pthread_create(&thread2, NULL, printBar, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
