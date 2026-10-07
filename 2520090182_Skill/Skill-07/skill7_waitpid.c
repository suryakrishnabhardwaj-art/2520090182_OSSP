#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    printf("====================================\n");
    printf(" SKILL 7 - PROCESS SYNCHRONIZATION\n");
    printf("====================================\n");

    printf("Parent Process PID: %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("\nChild Process Created\n");
        printf("Child PID: %d\n", getpid());
        printf("Child is executing...\n");

        sleep(2);

        printf("Child Process Completed\n");
        exit(0);
    }
    else
    {
        printf("\nParent is waiting for Child...\n");
        printf("Child PID: %d\n", pid);

        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            printf("\nChild process terminated normally.\n");
            printf("Child Exit Status: %d\n", WEXITSTATUS(status));
        }

        printf("Parent Process Completed\n");
    }

    printf("====================================\n");

    return 0;
}
