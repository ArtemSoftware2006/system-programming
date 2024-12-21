#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *array;
    size_t start;
    size_t end;
    double partial_sum;
} ThreadData;

void *compute_partial_sum(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->partial_sum = 0;

    for (size_t i = data->start; i < data->end; i++) {
        data->partial_sum += data->array[i];
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.bin output.bin\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    // Открытие входного файла
    FILE *file = fopen(input_file, "rb");
    if (!file) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    // Чтение данных из файла
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    size_t num_elements = file_size / sizeof(int);
    int *array = (int *)malloc(file_size);
    if (!array) {
        perror("Failed to allocate memory for array");
        fclose(file);
        return EXIT_FAILURE;
    }
    fread(array, sizeof(int), num_elements, file);
    fclose(file);

    // Определение количества потоков
    const int num_threads = 4; // Можно настроить
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    size_t chunk_size = num_elements / num_threads;

    // Создание потоков
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].array = array;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? num_elements : (i + 1) * chunk_size;

        pthread_create(&threads[i], NULL, compute_partial_sum, &thread_data[i]);
    }

    // Ожидание завершения потоков и суммирование результатов
    double total_sum = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].partial_sum;
    }

    double average = total_sum / num_elements;

    // Запись результата в файл
    file = fopen(output_file, "wb");
    if (!file) {
        perror("Failed to open output file");
        free(array);
        return EXIT_FAILURE;
    }
    fwrite(&average, sizeof(double), 1, file);
    fclose(file);

    // Очистка памяти
    free(array);

    printf("Average calculated: %f\n", average);
    return EXIT_SUCCESS;
}

