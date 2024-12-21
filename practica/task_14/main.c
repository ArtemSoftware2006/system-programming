#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Глобальная переменная для хранения суммы и мьютекс для её защиты
int sum = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;                 

// Функция для потока ввода
void* input_thread(void* arg) {
    int num;
    while (1) {
        printf("\rВведите число: ");
        fflush(stdout);
        if (scanf("%d", &num) != 1) { // Если ввод некорректен
            printf("Некорректный ввод. Завершение программы.\n");
            exit(0);
        }

        // Обновляем сумму с защитой мьютексом
        pthread_mutex_lock(&mutex);
        sum += num;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Функция для потока вывода
void* output_thread(void* arg) {
    while (1) {
        // Блокируем мьютекс для безопасного доступа к сумме
        pthread_mutex_lock(&mutex);

        pthread_cond_wait(&cond, &mutex);
        printf("\rСумма: %d", sum);
        fflush(stdout); // Обеспечивает немедленный вывод
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t input_tid, output_tid;

    // Инициализация мьютекса
    if (pthread_mutex_init(&mutex, NULL) != 0 || pthread_cond_init(&cond, NULL) != 0) {
        perror("Ошибка инициализации мьютекса");
        return EXIT_FAILURE;
    }

    // Создание потоков
    if (pthread_create(&input_tid, NULL, input_thread, NULL) != 0) {
        perror("Ошибка создания потока ввода");
        return EXIT_FAILURE;
    }
    if (pthread_create(&output_tid, NULL, output_thread, NULL) != 0) {
        perror("Ошибка создания потока вывода");
        return EXIT_FAILURE;
    }

    // Ожидание завершения потоков (программа в данной реализации будет бесконечной)
    pthread_join(input_tid, NULL);
    pthread_join(output_tid, NULL);

    // Освобождение ресурсов
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return EXIT_SUCCESS;
}
