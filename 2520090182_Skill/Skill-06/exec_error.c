#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    printf("Child process attempting to execute a program...\n");

    execlp("wrong_command", "wrong_command", NULL);

    perror("Execution Error");

    return 1;
}
