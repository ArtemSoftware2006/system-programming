#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int* array;
    int start;
    int end;
    double partial_sum;
} ThreadData;

void* ComputePart(void* arg) {
    ThreadData *data = (ThreadData*)arg;
    data->partial_sum = 0;

    for(int i = data->start; i < data->end; i++) {
        data->partial_sum += data->array[i];
    }
    return NULL;
}

#define INPUT_ARRAY_SIZE 10

int main() {
    const int NUMBER_THREADS = 2;
    pthread_t threads[NUMBER_THREADS];
    ThreadData threadsData[NUMBER_THREADS];

    int inputData[INPUT_ARRAY_SIZE] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    int chunk_size = INPUT_ARRAY_SIZE / NUMBER_THREADS;

    for(int i = 0; i < NUMBER_THREADS; i++) {
        threadsData[i].array = inputData;
        threadsData[i].start = i * chunk_size;
        if (i == NUMBER_THREADS - 1)
        {
            threadsData[i].end = INPUT_ARRAY_SIZE;
        } else {
            threadsData[i].end = chunk_size * (i + 1);
        }         

        pthread_create(&threads[i], NULL, ComputePart, &threadsData[i]);
    }

    double totla_sum = 0;

    for (int i = 0; i < NUMBER_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        totla_sum += threadsData[i].partial_sum;
    }

    double result = totla_sum / INPUT_ARRAY_SIZE;
    
    FILE* file = fopen("result.txt", "w");
    if (!file)
    {
        perror("Faild open file - result.txt");
        return 1;
    }
    fprintf(file, "%.6f\n", result);
    fclose(file);

    return 0;
}