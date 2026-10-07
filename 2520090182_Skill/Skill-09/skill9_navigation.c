#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

int main() {
    char current[PATH_MAX];
    char previous[PATH_MAX] = "";
    char path[PATH_MAX];

    if (getcwd(current, sizeof(current)) == NULL) {
        perror("getcwd");
        return 1;
    }

    printf("=== Directory Navigation ===\n");
    printf("Current Directory: %s\n", current);

    while (1) {
        printf("\nEnter directory path");
        printf(" (or 'prev' to go back, 'pwd' to display, 'exit' to quit): ");

        if (fgets(path, sizeof(path), stdin) == NULL)
            break;

        path[strcspn(path, "\n")] = '\0';

        if (strcmp(path, "exit") == 0) {
            printf("Exiting navigation...\n");
            break;
        }

        if (strcmp(path, "pwd") == 0) {
            if (getcwd(current, sizeof(current)) != NULL)
                printf("Current Directory: %s\n", current);
            continue;
        }

        if (strcmp(path, "prev") == 0) {
            if (strlen(previous) == 0) {
                printf("No previous directory available.\n");
                continue;
            }

            char temp[PATH_MAX];
            strcpy(temp, current);

            if (chdir(previous) == 0) {
                strcpy(previous, temp);

                if (getcwd(current, sizeof(current)) != NULL)
                    printf("Changed to previous directory: %s\n", current);
            } else {
                perror("Error changing to previous directory");
            }

            continue;
        }

        if (access(path, F_OK) != 0) {
            printf("Error: Path does not exist.\n");
            continue;
        }

        if (chdir(path) != 0) {
            perror("Error changing directory");
            continue;
        }

        strcpy(previous, current);

        if (getcwd(current, sizeof(current)) != NULL)
            printf("Directory changed successfully.\n");
            printf("Current Directory: %s\n", current);
    }

    return 0;
}
