#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_INPUT 100

void cmd_pwd() {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("Current Directory: %s\n", cwd);
    else
        perror("pwd");
}

void cmd_cd(char *path) {
    if (path == NULL) {
        printf("cd: missing operand\n");
        return;
    }

    if (chdir(path) != 0)
        perror("cd");
    else
        printf("Directory changed successfully.\n");
}

void cmd_help() {
    printf("\nBuilt-in Commands:\n");
    printf("  pwd   - Display current directory\n");
    printf("  cd    - Change directory\n");
    printf("  help  - Display available commands\n");
    printf("  exit  - Exit the dispatcher\n");
}

void cmd_exit() {
    printf("Exiting command dispatcher...\n");
}

int main() {
    char input[MAX_INPUT];
    char *command;
    char *argument;

    printf("=== Built-in Command Dispatcher ===\n");

    while (1) {
        printf("\ndispatch> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        command = strtok(input, " ");
        argument = strtok(NULL, "");

        if (command == NULL)
            continue;

        if (strcmp(command, "pwd") == 0) {
            cmd_pwd();
        }
        else if (strcmp(command, "cd") == 0) {
            cmd_cd(argument);
        }
        else if (strcmp(command, "help") == 0) {
            cmd_help();
        }
        else if (strcmp(command, "exit") == 0) {
            cmd_exit();
            break;
        }
        else {
            printf("Invalid command: %s\n", command);
            printf("Type 'help' to see available commands.\n");
        }
    }

    return 0;
}
