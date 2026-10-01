#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int *buffer;
int buffer_size;
int items;

int in = 0, out = 0;
long long consumed_sum = 0;
int consumed_count = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void *producer(void *arg)
{
    for (int i = 1; i <= items; i++)
    {
        sem_wait(&empty);

        pthread_mutex_lock(&mutex);

        buffer[in] = i;
        in = (in + 1) % buffer_size;

        pthread_mutex_unlock(&mutex);

        sem_post(&full);
    }

    return NULL;
}

void *consumer(void *arg)
{
    for (int i = 1; i <= items; i++)
    {
        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        int value = buffer[out];
        out = (out + 1) % buffer_size;

        consumed_sum += value;
        consumed_count++;

        pthread_mutex_unlock(&mutex);

        sem_post(&empty);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <buffer_size> <items>\n", argv[0]);
        return 1;
    }

    buffer_size = atoi(argv[1]);
    items = atoi(argv[2]);

    if (buffer_size <= 0 || items <= 0)
    {
        printf("Buffer size and items must be positive.\n");
        return 1;
    }

    buffer = malloc(buffer_size * sizeof(int));

    if (buffer == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    pthread_t producer_thread, consumer_thread;

    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    long long expected_sum = ((long long)items * (items + 1)) / 2;
    double throughput = items / elapsed;

    printf("\n===== PRODUCER-CONSUMER RESULT =====\n");
    printf("Buffer Size       : %d\n", buffer_size);
    printf("Items Produced    : %d\n", items);
    printf("Items Consumed    : %d\n", consumed_count);
    printf("Expected Sum      : %lld\n", expected_sum);
    printf("Consumed Sum      : %lld\n", consumed_sum);
    printf("Execution Time    : %.6f seconds\n", elapsed);
    printf("Throughput        : %.2f items/second\n", throughput);

    if (consumed_count == items && consumed_sum == expected_sum)
        printf("Synchronization   : CORRECT\n");
    else
        printf("Synchronization   : ERROR\n");

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    free(buffer);

    return 0;
}
