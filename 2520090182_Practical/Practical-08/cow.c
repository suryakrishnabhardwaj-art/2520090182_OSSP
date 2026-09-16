#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int *data = (int *)malloc(sizeof(int));

    *data = 100;

    printf("Before fork:\n");
    printf("Parent: value = %d, address = %p\n",
           *data, (void *)data);

    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed.\n");
        free(data);
        return 1;
    }

    if (pid == 0) {
        printf("\nChild before modification:\n");
        printf("Child: value = %d, address = %p\n",
               *data, (void *)data);

        *data = 200;

        printf("Child after modification:\n");
        printf("Child: value = %d, address = %p\n",
               *data, (void *)data);

        free(data);
    }
    else {
        wait(NULL);

        printf("\nParent after child modification:\n");
        printf("Parent: value = %d, address = %p\n",
               *data, (void *)data);

        free(data);
    }

    return 0;
}
