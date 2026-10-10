
# OS Skill 23: Pipeline Execution and Monitoring

## 1. Objective
Execute multiple pipeline jobs, monitor process completion,
measure CPU and memory usage, detect failures, and document
the system architecture and execution flow.

## 2. System Architecture
The program uses a parent process to launch five child processes.
Each child executes a shell command. Successful jobs run a
three-stage pipeline, while Job 4 intentionally fails.
The parent waits for all children, checks their exit statuses,
and reports resource usage and job statistics.

## 3. Architecture Flow Diagram

```text
Start
  |
  v
Initialize monitor
  |
  v
Launch five child jobs using fork()
  |
  v
Execute shell pipelines using execl()
  |
  v
Wait for jobs using waitpid()
  |
  v
Check exit statuses
  |
  v
Collect resource statistics
  |
  v
Report performance and stability
  |
  v
End
```

## 4. Execution Paths
- Parent path: creates jobs, waits for completion, collects
  resource statistics, and prints the summary.
- Successful child path: executes printf | tr | sed and exits
  successfully.
- Failure child path: Job 4 prints a failure message and exits
  with status 2.

## 5. System Calls and Functions
- fork(): creates a child process.
- execl(): replaces the child process with a shell.
- waitpid(): waits for a specific child process.
- getrusage(): retrieves resource usage for child processes.
- _exit(): terminates a child process immediately.
- perror(): reports system-call errors.

Supporting C library functions:
- clock(): measures CPU time consumed by the parent process.
- snprintf(): formats shell commands.
- printf(): displays monitoring information.

## 6. Resource Monitoring
The program reports child-process user CPU time, system CPU
time, and maximum resident memory. Additional Linux commands
such as free and uptime can be used to inspect system memory
and load.

## 7. Stability and Failure Detection
Five jobs are launched. Four should complete successfully.
Job 4 deliberately exits with a non-zero status. The parent
detects and reports this failure. The final program exit
status is 1 because a job failed.

## 8. Performance Analysis
The program reports the parent's CPU time and child resource
usage. clock() measures CPU time, not real elapsed wall-clock
time. For real elapsed time, use the Linux time command.

## 9. Design Decisions
- Use fork() to run jobs concurrently.
- Use waitpid() to collect individual job results.
- Use a controlled failure to test error detection.
- Use getrusage() to gather resource statistics.
- Keep the program small and reproducible for laboratory testing.

## 10. Review and Limitations
The program demonstrates basic concurrent job execution and
monitoring. The shell runs each successful three-stage pipeline.
The resource figures are aggregate child-process statistics,
not separate measurements for every job. The test does not
simulate production-scale workloads.

## 11. Compilation and Execution
```bash
gcc -Wall -Wextra pipeline_monitor.c -o pipeline_monitor
./pipeline_monitor
```

## 12. Conclusion
The program demonstrates multiple job execution, pipeline
processing, resource monitoring, failure detection, and
performance reporting using Linux process-management facilities.
