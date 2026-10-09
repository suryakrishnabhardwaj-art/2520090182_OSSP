#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    pid_t pgid;
    int status;

    printf("OS Skill-17: Foreground Job Control\n");

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        setpgid(0, 0);

        printf("Child process started. PID = %d\n", getpid());
        printf("Child process is running in foreground...\n");

        sleep(3);

        printf("Child process completed.\n");
        exit(0);
    }

    pgid = pid;

    setpgid(pid, pgid);

    printf("Parent identified target job PID = %d\n", pid);
    printf("Transferring terminal control to foreground job...\n");

    if (tcsetpgrp(STDIN_FILENO, pgid) == -1) {
        perror("tcsetpgrp");
    }

    waitpid(pid, &status, 0);

    printf("Foreground job completed.\n");
    printf("Updating job state...\n");

    if (WIFEXITED(status)) {
        printf("Job state: Completed\n");
    }
    else if (WIFSIGNALED(status)) {
        printf("Job state: Terminated\n");
    }

    if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1) {
        perror("tcsetpgrp");
    }

    printf("Terminal control returned to parent.\n");
    printf("Foreground switching test completed successfully.\n");

    return 0;
}
