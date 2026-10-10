
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *message = malloc(50);

    if (message == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    strcpy(message, "OS Skill 22: Memory Debugging");
    printf("%s\n", message);

    /* Intentional bug: memory is not freed. */
    return 0;
}
