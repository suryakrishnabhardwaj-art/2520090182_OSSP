#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
void handle_sigchld(int sig)
{
printf("SIGCHLD received: Child has terminated.\n");
}
int main()
{
signal(SIGCHLD, handle_sigchld);
pid_t pid = fork();
if (pid == 0)
{
// Child
printf("Child is running...\n");
sleep(3);
printf("Child is terminating...\n");
}
else
{
// Parent
printf("Parent is waiting...\n");
wait(NULL);
printf("Parent finished.\n");
}
return 0;
}
