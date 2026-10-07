#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[100];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process - reads from pipe
        close(pipefd[1]);

        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        close(pipefd[0]);
        exit(0);
    } 
    else {
        // Parent process - writes to pipe
        close(pipefd[0]);

        char message[] = "Hello from Parent through Pipe";

        write(pipefd[1], message, strlen(message) + 1);

        close(pipefd[1]);

        wait(NULL);

        printf("Parent: Child process completed.\n");
    }

    return 0;
}
