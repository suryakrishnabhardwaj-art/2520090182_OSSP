#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 20
#define MAX_HISTORY 10

int main() {
    char **history = malloc(MAX_HISTORY * sizeof(char *));
    int history_count = 0;

    char *input = malloc(INITIAL_SIZE);
    int buffer_size = INITIAL_SIZE;

    if (history == NULL || input == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("===== COMMAND HISTORY DEMO =====\n");
    printf("Escape sequence example: \\033[A = UP arrow\n");
    printf("Escape sequence example: \\033[B = DOWN arrow\n\n");

    /* Store commands */
    const char *commands[] = {
        "ls",
        "pwd",
        "gcc program.c",
        "git status",
        "git push"
    };

    for (int i = 0; i < 5; i++) {
        history[history_count] = malloc(strlen(commands[i]) + 1);

        if (history[history_count] == NULL) {
            printf("Memory allocation failed!\n");
            return 1;
        }

        strcpy(history[history_count], commands[i]);
        history_count++;
    }

    printf("Commands stored in history:\n");

    for (int i = 0; i < history_count; i++) {
        printf("%d. %s\n", i + 1, history[i]);
    }

    /* Resize input buffer */
    buffer_size *= 2;
    input = realloc(input, buffer_size);

    if (input == NULL) {
        printf("Buffer resizing failed!\n");
        return 1;
    }

    strcpy(input, "git status");

    printf("\nInput buffer size after resize: %d bytes\n", buffer_size);
    printf("Current input buffer: %s\n", input);

    /* Recall previous command */
    printf("\nPrevious command recalled using UP arrow:\n");
    printf("%s\n", history[history_count - 2]);

    /* Recall next command */
    printf("\nNext command recalled using DOWN arrow:\n");
    printf("%s\n", history[history_count - 1]);

    /* Update input buffer */
    strcpy(input, history[history_count - 1]);

    printf("\nUpdated input buffer:\n");
    printf("%s\n", input);

    /* Free memory */
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }

    free(history);
    free(input);

    printf("\nAll dynamically allocated memory released successfully.\n");

    return 0;
}
