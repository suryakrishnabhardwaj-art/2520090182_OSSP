#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMANDS 2

void validate_pipeline(char *commands[])
{
    printf("Validating pipeline...\n");

    for (int i = 0; i < MAX_COMMANDS; i++)
    {
        if (commands[i] == NULL || strlen(commands[i]) == 0)
        {
            printf("Pipeline validation: FAILED\n");
            return;
        }
    }

    printf("Pipeline validation: SUCCESS\n");
}

int main()
{
    int pipefd[2];
    pid_t pid1, pid2;

    char *commands[MAX_COMMANDS] = {
        "ls",
        "wc -l"
    };

    printf("OS Skill-11: Pipeline Management\n");

    printf("\nPipeline commands:\n");
    printf("1. %s\n", commands[0]);
    printf("2. %s\n", commands[1]);

    printf("\nExecution order:\n");
    printf("Process 1 -> Process 2\n");

    validate_pipeline(commands);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid1 = fork();

    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[0]);
        close(pipefd[1]);

        execlp("ls", "ls", NULL);

        perror("execlp");
        exit(1);
    }

    pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid2 == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);

        close(pipefd[0]);
        close(pipefd[1]);

        execlp("wc", "wc", "-l", NULL);

        perror("execlp");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Pipeline execution completed successfully.\n");

    return 0;
}
