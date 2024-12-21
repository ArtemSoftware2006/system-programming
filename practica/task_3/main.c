#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// Структура для передачи данных в поток
typedef struct {
    int thread_id;
    int *last_finished_thread;
    pthread_mutex_t *mutex;
} ThreadData;

// Функция потока
void* ThreadFunction(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    // Поток засыпает на случайное время
    int sleep_time = rand() % 1000; // Случайное время сна
    usleep(sleep_time * 1000);      // Перевод в микросекунды

    // Защищаем доступ к общей переменной
    pthread_mutex_lock(data->mutex);
    *(data->last_finished_thread) = data->thread_id;
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

int main() {
    const int NUM_THREADS = 100; // Количество потоков
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    pthread_mutex_t mutex;
    int last_finished_thread = -1;

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Инициализация мьютекса
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    // Создание потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i + 1;  // Номер потока
        thread_data[i].last_finished_thread = &last_finished_thread;
        thread_data[i].mutex = &mutex;

        if (pthread_create(&threads[i], NULL, ThreadFunction, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Вывод результата
    printf("The last finished thread is: %d\n", last_finished_thread);

    // Уничтожение мьютекса
    pthread_mutex_destroy(&mutex);

    return 0;
}
