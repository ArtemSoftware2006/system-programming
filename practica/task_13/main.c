#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // Размер буфера

int buffer[BUFFER_SIZE];  // Буфер для хранения данных
int count = 0;            // Текущее количество элементов в буфере
int in = 0;               // Указатель на место для вставки данных
int out = 0;              // Указатель на место для извлечения данных

pthread_mutex_t mutex;    // Мьютекс для защиты доступа к буферу
pthread_cond_t not_empty; // Условная переменная: буфер не пуст
pthread_cond_t not_full;  // Условная переменная: буфер не полный

// Функция для производителей
void* producer(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        int item = rand() % 100; // Производим случайное число
        usleep(500000);          // Производим медленнее, чем потребляют

        pthread_mutex_lock(&mutex);

        // Ждём, пока в буфере появится место
        while (count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        // Кладём данные в буфер
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Производитель %d произвел: %d\n", id, item);

        // Уведомляем потребителей
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Функция для потребителей
void* consumer(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);

        // Ждём, пока в буфере появятся данные
        while (count == 0)
            pthread_cond_wait(&not_empty, &mutex);

        // Забираем данные из буфера
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Потребитель %d потребил: %d\n", id, item);

        // Уведомляем производителей
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        usleep(200000); // Потребляем быстрее, чем производят
    }

    return NULL;
}

int main() {
    int producers, consumers;

    printf("Введите количество производителей: ");
    scanf("%d", &producers);
    printf("Введите количество потребителей: ");
    scanf("%d", &consumers);

    pthread_t producer_threads[producers];
    pthread_t consumer_threads[consumers];

    // Инициализация мьютекса и условных переменных
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    // Создание потоков производителей
    for (int i = 0; i < producers; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&producer_threads[i], NULL, producer, id);
    }

    // Создание потоков потребителей
    for (int i = 0; i < consumers; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consumer_threads[i], NULL, consumer, id);
    }

    // Ожидание завершения потоков (никогда не завершатся)
    for (int i = 0; i < producers; i++) {
        pthread_join(producer_threads[i], NULL);
    }
    for (int i = 0; i < consumers; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Освобождение ресурсов
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
