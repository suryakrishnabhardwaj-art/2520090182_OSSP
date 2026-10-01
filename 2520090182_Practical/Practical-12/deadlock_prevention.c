#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource1;
pthread_mutex_t resource2;

void *thread1(void *arg)
{
    printf("Thread 1: Requesting Resource 1...\n");
    pthread_mutex_lock(&resource1);

    printf("Thread 1: Resource 1 acquired.\n");

    sleep(1);

    printf("Thread 1: Requesting Resource 2...\n");
    pthread_mutex_lock(&resource2);

    printf("Thread 1: Resource 2 acquired.\n");
    printf("Thread 1: Working with both resources.\n");

    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);

    printf("Thread 1: Resources released.\n");

    return NULL;
}

void *thread2(void *arg)
{
    printf("Thread 2: Requesting Resource 1...\n");
    pthread_mutex_lock(&resource1);

    printf("Thread 2: Resource 1 acquired.\n");

    sleep(1);

    printf("Thread 2: Requesting Resource 2...\n");
    pthread_mutex_lock(&resource2);

    printf("Thread 2: Resource 2 acquired.\n");
    printf("Thread 2: Working with both resources.\n");

    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);

    printf("Thread 2: Resources released.\n");

    return NULL;
}

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&resource1, NULL);
    pthread_mutex_init(&resource2, NULL);

    printf("===== DEADLOCK PREVENTION =====\n");
    printf("Resource ordering: Resource 1 -> Resource 2\n\n");

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nDeadlock prevented using resource ordering.\n");

    pthread_mutex_destroy(&resource1);
    pthread_mutex_destroy(&resource2);

    return 0;
}
