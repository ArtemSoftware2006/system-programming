#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS_COUNT 3
#define MAX_VALUE 900
#define THRESHOLD_1 300
#define THRESHOLD_2 600
#define THRESHOLD_3 900

// Структура для данных потока
typedef struct {
    int thread_id;
    int* counter;
    pthread_mutex_t* mutex;
    pthread_barrier_t* barrier_300;
    pthread_barrier_t* barrier_600;
    pthread_barrier_t* barrier_900; 
} ThreadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* count(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    int current_value = 0;
    while (current_value < MAX_VALUE) {
        pthread_mutex_lock(data->mutex);
        
        // Увеличиваем счётчик с разной скоростью
        current_value += 1 + data->thread_id;  // Потоки увеличивают счётчик с разной скоростью (по 1, 2, 3)

        pthread_mutex_unlock(data->mutex);

        // Выводим текущее состояние потока
        printf("Thread %d: current value = %d\n", data->thread_id, current_value);

        int cond_1, cond_2, cond_3 = 0;

        // Синхронизация на 300
        if (current_value >= THRESHOLD_1 && current_value < THRESHOLD_2 && cond_1 == 0) {
            printf("Thread %d reached threshold 300\n", data->thread_id);
            pthread_barrier_wait(data->barrier_300);  // Барьер для 300
            cond_1 = 1;
        }

        // Синхронизация на 600
        if (current_value >= THRESHOLD_2 && current_value < THRESHOLD_3 && cond_2 == 0) {
            printf("Thread %d reached threshold 600\n", data->thread_id);
            pthread_barrier_wait(data->barrier_600);  // Барьер для 600
            cond_2 = 1;
        }

        // Синхронизация на 900
        if (current_value >= THRESHOLD_3 && cond_3 == 0) {
            printf("Thread %d reached threshold 900\n", data->thread_id);
            pthread_barrier_wait(data->barrier_900);  // Барьер для 900
            cond_3 = 1;
        }
    }

    return NULL;
}

int main() {
    pthread_t threads[THREADS_COUNT];
    ThreadData thread_data[THREADS_COUNT];

    // Инициализация барьеров
    pthread_barrier_t barrier_300, barrier_600, barrier_900;
    pthread_barrier_init(&barrier_300, NULL, THREADS_COUNT);
    pthread_barrier_init(&barrier_600, NULL, THREADS_COUNT);
    pthread_barrier_init(&barrier_900, NULL, THREADS_COUNT);

    // Инициализация данных для каждого потока
    for (int i = 0; i < THREADS_COUNT; i++) {
        thread_data[i].thread_id = i + 1;
        thread_data[i].counter = (int*)malloc(sizeof(int));  // Место для счётчика
        *(thread_data[i].counter) = 0;
        thread_data[i].mutex = &mutex;
        thread_data[i].barrier_300 = &barrier_300;
        thread_data[i].barrier_600 = &barrier_600;
        thread_data[i].barrier_900 = &barrier_900;

        pthread_create(&threads[i], NULL, count, (void*)&thread_data[i]);
    }

    // Ожидаем завершения всех потоков
    for (int i = 0; i < THREADS_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Очистка барьеров
    pthread_barrier_destroy(&barrier_300);
    pthread_barrier_destroy(&barrier_600);
    pthread_barrier_destroy(&barrier_900);

    printf("Final state reached.\n");

    return 0;
}
