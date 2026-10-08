#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_JOBS 10

typedef enum {
    RUNNING,
    DONE
} JobState;

typedef struct {
    int job_id;
    pid_t pid;
    pid_t pgid;
    JobState state;
    int duration;
} Job;

Job job_table[MAX_JOBS];
int job_count = 0;
int next_job_id = 1;

/* Update status of all background jobs */
void update_jobs() {
    for (int i = 0; i < job_count; i++) {

        if (job_table[i].state == RUNNING) {

            int status;
            pid_t result = waitpid(
                job_table[i].pid,
                &status,
                WNOHANG
            );

            if (result > 0) {
                job_table[i].state = DONE;
                printf("\n[Job %d] PID %d completed.\n",
                       job_table[i].job_id,
                       job_table[i].pid);
            }
        }
    }
}

/* Remove completed jobs from the job table */
void remove_completed_jobs() {
    int new_count = 0;

    for (int i = 0; i < job_count; i++) {

        if (job_table[i].state == RUNNING) {
            job_table[new_count++] = job_table[i];
        } else {
            printf("Removing completed Job %d (PID %d)\n",
                   job_table[i].job_id,
                   job_table[i].pid);
        }
    }

    job_count = new_count;
}

/* Display current job table */
void display_jobs() {
    update_jobs();

    printf("\n========== JOB TABLE ==========\n");

    if (job_count == 0) {
        printf("No active jobs.\n");
        printf("===============================\n");
        return;
    }

    printf("ID\tPID\tPGID\tSTATE\t\tDURATION\n");

    for (int i = 0; i < job_count; i++) {

        printf("%d\t%d\t%d\t%s\t\t%d sec\n",
               job_table[i].job_id,
               job_table[i].pid,
               job_table[i].pgid,
               job_table[i].state == RUNNING
                   ? "RUNNING"
                   : "DONE",
               job_table[i].duration);
    }

    printf("===============================\n");
}

/* Launch a non-blocking background job */
void launch_background_job(int duration) {

    if (job_count >= MAX_JOBS) {
        printf("Job table is full.\n");
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {

        /* Child creates its own process group */
        setpgid(0, 0);

        printf("Child process PID %d started.\n", getpid());

        sleep(duration);

        printf("Child process PID %d finished.\n", getpid());

        exit(0);
    }

    /* Parent stores job information */
    setpgid(pid, pid);

    job_table[job_count].job_id = next_job_id++;
    job_table[job_count].pid = pid;
    job_table[job_count].pgid = pid;
    job_table[job_count].state = RUNNING;
    job_table[job_count].duration = duration;

    printf("[Job %d] Started background process PID=%d PGID=%d\n",
           job_table[job_count].job_id,
           pid,
           pid);

    job_count++;

    printf("Prompt returned immediately. Parent is not blocked.\n");
}

int main() {

    char command[100];
    int duration;

    printf("=====================================\n");
    printf("   OS SKILL-16: BACKGROUND JOBS\n");
    printf("=====================================\n");

    printf("Commands:\n");
    printf("  run <seconds>  - Start background job\n");
    printf("  jobs           - Display job table\n");
    printf("  cleanup        - Remove completed jobs\n");
    printf("  exit           - Exit program\n");

    while (1) {

        update_jobs();

        printf("\nbackground-shell> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            break;
        }

        else if (strcmp(command, "jobs") == 0) {
            display_jobs();
        }

        else if (strcmp(command, "cleanup") == 0) {
            update_jobs();
            remove_completed_jobs();
        }

        else if (sscanf(command, "run %d", &duration) == 1) {

            if (duration <= 0) {
                printf("Duration must be greater than 0.\n");
            } else {
                launch_background_job(duration);
            }
        }

        else {
            printf("Invalid command.\n");
            printf("Use: run <seconds>, jobs, cleanup, exit\n");
        }
    }

    /* Clean up remaining children */
    for (int i = 0; i < job_count; i++) {

        if (job_table[i].state == RUNNING) {
            waitpid(job_table[i].pid, NULL, 0);
        }
    }

    printf("\nAll background jobs cleaned up.\n");
    printf("Program terminated.\n");

    return 0;
}
