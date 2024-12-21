#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

pthread_cond_t is_value_changed;
pthread_cond_t is_value_output;
pthread_mutex_t mutex;
int value = 0;

void* observer(void* args) {
    while(1) {
        pthread_mutex_lock(&mutex);

        // Ожидаем, пока не будет изменено значение
        pthread_cond_wait(&is_value_changed, &mutex);

        // Выводим значение
        printf("Value is changed: %d\n", value);

        // Сигнализируем, что значение выведено
        pthread_cond_signal(&is_value_output);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* changer(void* args) {
    int id = rand() % 10;
    while(1) {
        pthread_mutex_lock(&mutex);

        // Ждем, пока observer не выведет значение
        pthread_cond_wait(&is_value_output, &mutex);

        // Меняем значение
        value = rand() % 100;
        printf("Changed by %d\n", id);

        // Сигнализируем, что значение изменено
        pthread_cond_signal(&is_value_changed);

        pthread_mutex_unlock(&mutex);

        usleep(500000); // Пауза перед следующим изменением
    }

    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&is_value_changed, NULL);
    pthread_cond_init(&is_value_output, NULL);

    int N;
    printf("Input number of threads:\n");
    scanf("%d", &N);
    N = N + 1; // Один поток для observer, остальные для changer

    pthread_t threads[N];

    // Создаем поток observer
    pthread_create(&threads[0], NULL, observer, NULL);

    // Создаем потоки changer
    for (int i = 1; i < N; i++) {
        pthread_create(&threads[i], NULL, changer, NULL);
    }

    // Обеспечим старт для первого потока changer (он должен начать изменять значение сразу)
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&is_value_output);
    pthread_mutex_unlock(&mutex);

    // Ожидаем завершения всех потоков
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&is_value_changed);
    pthread_cond_destroy(&is_value_output);

    return 0;
}
