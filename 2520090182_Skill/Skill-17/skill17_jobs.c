#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_JOBS 10

typedef struct {
    int job_id;
    pid_t pid;
    char state[20];
} Job;

Job jobs[MAX_JOBS];
int job_count = 0;

void add_job(pid_t pid)
{
    jobs[job_count].job_id = job_count + 1;
    jobs[job_count].pid = pid;
    snprintf(jobs[job_count].state, sizeof(jobs[job_count].state), "Running");
    job_count++;
}

void update_jobs()
{
    for (int i = 0; i < job_count; i++) {
        int status;
        pid_t result = waitpid(jobs[i].pid, &status, WNOHANG);

        if (result == jobs[i].pid) {
            if (WIFEXITED(status))
                snprintf(jobs[i].state, sizeof(jobs[i].state), "Completed");
            else if (WIFSIGNALED(status))
                snprintf(jobs[i].state, sizeof(jobs[i].state), "Terminated");
        }
    }
}

void list_jobs()
{
    update_jobs();

    printf("\n%-8s %-10s %-15s\n", "Job ID", "PID", "Status");
    printf("----------------------------------\n");

    for (int i = 0; i < job_count; i++) {
        printf("%-8d %-10d %-15s\n",
               jobs[i].job_id,
               jobs[i].pid,
               jobs[i].state);
    }
}

int main()
{
    printf("OS Skill-17: Job Listing and Status Information\n");

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            sleep(3 + i);
            exit(0);
        }
        else if (pid > 0) {
            add_job(pid);
            printf("Created Job ID %d with PID %d\n",
                   jobs[job_count - 1].job_id, pid);
        }
        else {
            perror("fork");
            return 1;
        }
    }

    printf("\nInitial Job Listing:");
    list_jobs();

    printf("\nWaiting for jobs to complete...\n");
    sleep(5);

    printf("\nUpdated Job Listing:");
    list_jobs();

    printf("\nJob listing test completed successfully.\n");

    return 0;
}
