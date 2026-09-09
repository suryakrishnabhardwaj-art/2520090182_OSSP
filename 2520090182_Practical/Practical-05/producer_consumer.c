#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N 100000

int main()
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid > 0)
    {
        // Parent process - Producer
        close(pipefd[0]);

        int buffer[1024];
        int count = 0;
        long long total_bytes = 0;

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 1; i <= N; i++)
        {
            buffer[count++] = i;

            if (count == 1024 || i == N)
            {
                ssize_t bytes = write(pipefd[1],
                                      buffer,
                                      count * sizeof(int));

                if (bytes == -1)
                {
                    perror("write");
                    close(pipefd[1]);
                    wait(NULL);
                    return 1;
                }

                total_bytes += bytes;
                count = 0;
            }
        }

        close(pipefd[1]);

        wait(NULL);

        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed =
            (end.tv_sec - start.tv_sec) +
            (end.tv_nsec - start.tv_nsec) / 1e9;

        double throughput =
            (total_bytes / (1024.0 * 1024.0)) / elapsed;

        printf("\nProducer: Generated %d integers.\n", N);
        printf("Producer: Sent %.2f KB through pipe.\n",
               total_bytes / 1024.0);
        printf("Communication Time: %.6f seconds\n", elapsed);
        printf("Communication Throughput: %.2f MB/s\n",
               throughput);
    }
    else
    {
        // Child process - Consumer
        close(pipefd[1]);

        int buffer[1024];
        ssize_t bytes;
        long long sum = 0;
        long long count = 0;

        while ((bytes = read(pipefd[0],
                             buffer,
                             sizeof(buffer))) > 0)
        {
            int numbers = bytes / sizeof(int);

            for (int i = 0; i < numbers; i++)
            {
                sum += buffer[i];
                count++;
            }
        }

        close(pipefd[0]);

        printf("Consumer: Received %lld integers.\n", count);
        printf("Consumer: Sum of received data = %lld\n", sum);
        exit(0);
    }

    return 0;
}
