#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipe1[2];
    int pipe2[2];

    pid_t p1, p2, p3;

    if (pipe(pipe1) == -1) {
        perror("pipe1");
        return 1;
    }

    if (pipe(pipe2) == -1) {
        perror("pipe2");
        return 1;
    }

    // First process
    p1 = fork();

    if (p1 == -1) {
        perror("fork");
        return 1;
    }

    if (p1 == 0) {
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        char message[] = "Data from Process 1";

        write(pipe1[1], message, strlen(message) + 1);

        close(pipe1[1]);
        exit(0);
    }

    // Second process
    p2 = fork();

    if (p2 == -1) {
        perror("fork");
        return 1;
    }

    if (p2 == 0) {
        char buffer[100];

        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], buffer, sizeof(buffer));

        char message[150];
        snprintf(message, sizeof(message),
                 "%s -> Process 2", buffer);

        write(pipe2[1], message, strlen(message) + 1);

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    // Third process
    p3 = fork();

    if (p3 == -1) {
        perror("fork");
        return 1;
    }

    if (p3 == 0) {
        char buffer[150];

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        read(pipe2[0], buffer, sizeof(buffer));

        printf("Process 3 received: %s\n", buffer);

        close(pipe2[0]);

        exit(0);
    }

    // Parent closes all pipe descriptors
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Synchronize all child processes
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    waitpid(p3, NULL, 0);

    printf("Parent: All processes completed successfully.\n");
    printf("Parent: Pipes closed and resources cleaned up.\n");

    return 0;
}
