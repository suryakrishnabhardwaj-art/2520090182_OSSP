
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t running = 1;

void handle_signal(int sig)
{
    if (sig == SIGTERM || sig == SIGINT) {
        running = 0;
    }
}

int main()
{
    pid_t pid;
    int status;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("OS Skill-18: Signal and Job Control\n");
    printf("Parent Process PID: %d\n", getpid());

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGTERM, SIG_DFL);

        printf("Child Process Started. PID: %d\n", getpid());

        while (1) {
            printf("Child process is running...\n");
            sleep(2);
        }

        exit(0);
    } else {
        printf("Child process created. PID: %d\n", pid);
        sleep(3);

        printf("\nSending SIGSTOP to child...\n");
        kill(pid, SIGSTOP);
        waitpid(pid, &status, WUNTRACED);

        if (WIFSTOPPED(status)) {
            printf("Child process stopped successfully.\n");
        }

        printf("\nSending SIGCONT to resume child...\n");
        kill(pid, SIGCONT);
        printf("Continue signal sent successfully.\n");

        sleep(3);

        printf("\nSending SIGTERM to terminate child...\n");
        kill(pid, SIGTERM);

        waitpid(pid, &status, 0);

        if (WIFSIGNALED(status)) {
            printf("Child terminated by signal: %d\n",
                   WTERMSIG(status));
        }

        printf("Parent process completed.\n");
    }

    return 0;
}
