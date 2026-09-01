#include <stdio.h>
#include <unistd.h>
int main()
{
int fd[2];
pipe(fd);
close(fd[0]); // Close read end
printf("Writing to pipe...\n");
write(fd[1], "Hello", 5);
printf("Write completed.\n");
return 0;
}
