#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void run_test(int order)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        close(pipefd[0]);

        /*
         * Redirect stdout to the pipe.
         */
        dup2(pipefd[1], STDOUT_FILENO);

        /*
         * Demonstrate ordering of:
         *
         * 2>&1
         *
         * If order == 1:
         * stdout -> pipe
         * stderr -> stdout -> pipe
         *
         * If order == 2:
         * stderr remains unchanged
         * stdout -> pipe
         */
        if (order == 1)
        {
            dup2(STDOUT_FILENO, STDERR_FILENO);
        }

        close(pipefd[1]);

        printf("Child: standard output message\n");
        fprintf(stderr, "Child: standard error message\n");

        exit(0);
    }
    else
    {
        close(pipefd[1]);

        char buffer[1024];
        int total = 0;
        int n;

        while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[n] = '\0';
            printf("%s", buffer);
            total += n;
        }

        close(pipefd[0]);

        waitpid(pid, NULL, 0);

        printf("Parent: received %d bytes through pipe\n", total);
    }
}

int main()
{
    printf("========================================\n");
    printf(" SKILL-15: REDIRECTION AND PIPE TEST\n");
    printf("========================================\n\n");

    printf("TEST 1: stdout and stderr merged using 2>&1\n");
    printf("----------------------------------------\n");

    run_test(1);

    printf("\nTEST 2: stdout redirected, stderr kept separate\n");
    printf("----------------------------------------\n");

    run_test(2);

    printf("\nSkill-15 execution completed successfully.\n");

    return 0;
}
