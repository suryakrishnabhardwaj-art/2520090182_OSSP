#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

volatile sig_atomic_t child_pid = 0;

void handle_signal(int sig)
{
    int saved_errno = errno;
    pid_t pid = (pid_t)child_pid;

    if (sig == SIGINT) {
        const char msg[] =
            "\nParent: SIGINT received. Forwarding to child.\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    }

    if (pid > 0)
        kill(pid, sig);

    errno = saved_errno;
}

int main(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("SIGINT handler registered successfully.\n");
    printf("Parent PID: %ld\n", (long)getpid());
    printf("Press Ctrl+C to interrupt the child.\n");
    fflush(stdout);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        struct sigaction child_sa;
        memset(&child_sa, 0, sizeof(child_sa));

        child_sa.sa_handler = SIG_DFL;
        sigemptyset(&child_sa.sa_mask);

        sigaction(SIGINT, &child_sa, NULL);

        printf("Child process started. PID: %ld\n",
               (long)getpid());
        fflush(stdout);

        for (int i = 1; i <= 30; i++) {
            printf("Child working... %d/30\n", i);
            fflush(stdout);
            sleep(1);
        }

        printf("Child completed normally.\n");
        fflush(stdout);
        _exit(0);
    }

    child_pid = (sig_atomic_t)pid;

    int status;
    pid_t result;

    do {
        result = waitpid(pid, &status, 0);
    } while (result == -1 && errno == EINTR);

    child_pid = 0;

    if (result == -1) {
        perror("waitpid");
        return 1;
    }

    if (WIFSIGNALED(status)) {
        printf("Child terminated by signal %d.\n",
               WTERMSIG(status));
    } else if (WIFEXITED(status)) {
        printf("Child exited with status %d.\n",
               WEXITSTATUS(status));
    }

    printf("Parent process is still running safely.\n");

    return 0;
}
