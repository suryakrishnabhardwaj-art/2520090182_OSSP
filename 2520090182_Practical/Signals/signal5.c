#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
void handler(int sig)
{
printf("Child received SIGUSR1\n");
}
int main()
{
pid_t pid = fork();
if (pid == 0)
{
// Child
signal(SIGUSR1, handler);
printf("Child waiting...\n");
while (1)
{
sleep(1);
}
}
else
{
// Parent
sleep(2);
printf("Parent sending signal...\n");
kill(pid, SIGUSR1);
sleep(2);
kill(pid, SIGTERM);
wait(NULL);
}
return 0;
}
