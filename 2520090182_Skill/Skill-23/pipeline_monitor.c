
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>

#define JOBS 5

int main(void)
{
    pid_t pids[JOBS];
    int status[JOBS];
    int success = 0, failed = 0;
    struct rusage usage;
    clock_t start, end;

    setbuf(stdout, NULL);

    printf("====================================\n");
    printf(" OS SKILL 23: PIPELINE MONITOR\n");
    printf("====================================\n");

    printf("Total jobs: %d\n", JOBS);
    printf("Launching jobs...\n");

    start = clock();

    /* Launch multiple jobs */
    for (int i = 0; i < JOBS; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork failed");
            failed++;
            pids[i] = -1;
            continue;
        }

        if (pids[i] == 0) {
            char command[256];

            if (i == 3) {
                /* Intentional failure for testing */
                snprintf(command, sizeof(command),
                         "echo 'Job 4: simulated failure' >&2; exit 2");
            } else {
                snprintf(command, sizeof(command),
                         "printf 'job-%d pipeline running\\n' | "
                         "tr 'a-z' 'A-Z' | sed 's/JOB/PIPELINE/'",
                         i + 1);
            }

            execl("/bin/sh", "sh", "-c", command, (char *)NULL);
            perror("execl failed");
            _exit(127);
        }

        printf("Launched job %d with PID %ld\n",
               i + 1, (long)pids[i]);
    }

    printf("\nMonitoring job completion...\n");

    /* Wait for every successfully created child */
    for (int i = 0; i < JOBS; i++) {
        if (pids[i] == -1)
            continue;

        pid_t result = waitpid(pids[i], &status[i], 0);

        if (result == -1) {
            perror("waitpid failed");
            failed++;
            continue;
        }

        if (WIFEXITED(status[i]) &&
            WEXITSTATUS(status[i]) == 0) {
            printf("Job %d: SUCCESS\n", i + 1);
            success++;
        } else {
            printf("Job %d: FAILURE detected\n", i + 1);
            failed++;
        }
    }

    end = clock();

    /* Resource usage of completed child processes */
    if (getrusage(RUSAGE_CHILDREN, &usage) == 0) {
        printf("\nResource Monitoring:\n");
        printf("User CPU time: %ld.%06ld seconds\n",
               (long)usage.ru_utime.tv_sec,
               (long)usage.ru_utime.tv_usec);
        printf("System CPU time: %ld.%06ld seconds\n",
               (long)usage.ru_stime.tv_sec,
               (long)usage.ru_stime.tv_usec);
        printf("Maximum resident memory: %ld KB\n",
               usage.ru_maxrss);
    }

    printf("\nPerformance Analysis:\n");
    printf("Elapsed CPU time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("\nStability Summary:\n");
    printf("Successful jobs: %d\n", success);
    printf("Failed jobs: %d\n", failed);
    printf("Total jobs: %d\n", success + failed);

    if (failed == 0)
        printf("Pipeline status: ALL JOBS PASSED\n");
    else
        printf("Pipeline status: FAILURES DETECTED\n");

    return failed > 0 ? 1 : 0;
}
