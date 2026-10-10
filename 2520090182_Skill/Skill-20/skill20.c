
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <errno.h>

volatile sig_atomic_t signal_received = 0;

void handle_usr1(int sig)
{
    signal_received = sig;
}

int main(void)
{
    pid_t child;
    int status;

    setbuf(stdout, NULL);

    printf("===== PART 1: SIGTSTP SUSPEND AND RESUME =====\n");

    child = fork();

    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        printf("Child process started. PID = %d\n", getpid());
        printf("Child is working; its state will be preserved.\n");

        for (int i = 1; i <= 5; i++) {
            printf("Child working: step %d\n", i);
            sleep(1);
        }

        printf("Child completed its work.\n");
        _exit(0);
    }

    sleep(2);
    printf("Parent sends SIGTSTP to child PID %d.\n", child);

    if (kill(child, SIGTSTP) == -1)
        perror("kill SIGTSTP");

    if (waitpid(child, &status, WUNTRACED) == -1) {
        perror("waitpid");
        return 1;
    }

    if (WIFSTOPPED(status)) {
        printf("Child suspended by signal %d.\n", WSTOPSIG(status));
        printf("Parent sends SIGCONT to resume the child.\n");

        if (kill(child, SIGCONT) == -1)
            perror("kill SIGCONT");

        if (waitpid(child, &status, 0) == -1)
            perror("waitpid after resume");

        printf("Child has finished; exit status collected.\n");
    } else {
        printf("Child was not stopped; suspension was not demonstrated.\n");
    }

    printf("\n===== PART 2: PROCESS GROUPS AND TERMINAL =====\n");

    child = fork();

    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        signal(SIGUSR1, handle_usr1);
        setpgid(0, 0);

        printf("Group child PID = %d, PGID = %d\n",
               getpid(), getpgrp());

        for (int i = 0; i < 50 && !signal_received; i++)
            usleep(100000);

        if (signal_received)
            printf("Child received SIGUSR1 from its process group.\n");

        _exit(0);
    }

    if (setpgid(child, child) == -1 && errno != EACCES &&
        errno != ESRCH)
        perror("setpgid");

    printf("Parent PID = %d, child PID = %d\n", getpid(), child);
    printf("Assigned child process group ID = %d\n", child);

    sleep(1);

    if (isatty(STDIN_FILENO)) {
        struct sigaction sa = {0};
        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGTTOU, &sa, NULL);

        pid_t shell_group = tcgetpgrp(STDIN_FILENO);

        if (shell_group != -1 && tcsetpgrp(STDIN_FILENO, child) == 0) {
            printf("Terminal foreground group temporarily transferred.\n");
            tcsetpgrp(STDIN_FILENO, getpgrp());
            printf("Terminal foreground group restored to parent group.\n");
        } else {
            printf("Terminal transfer unavailable; continuing group test.\n");
        }
    } else {
        printf("No interactive terminal; terminal transfer skipped.\n");
    }

    printf("Parent sends SIGUSR1 to process group %d.\n", child);

    if (kill(-child, SIGUSR1) == -1)
        perror("kill process group");

    if (waitpid(child, &status, 0) == -1)
        perror("waitpid group child");

    printf("Process group signal test completed.\n");
    printf("===== SKILL 20 COMPLETED =====\n");

    return 0;
}
