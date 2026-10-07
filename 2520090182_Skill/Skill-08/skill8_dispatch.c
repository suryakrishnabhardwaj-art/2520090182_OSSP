#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_INPUT 200

/* Built-in command functions */
void cmd_pwd(char *args)
{
    char path[200];

    if (getcwd(path, sizeof(path)) != NULL)
        printf("Current Directory: %s\n", path);
}

void cmd_echo(char *args)
{
    if (args != NULL)
        printf("%s\n", args);
}

void cmd_set(char *args)
{
    static char state[100] = "default";

    if (args == NULL || strlen(args) == 0)
    {
        printf("Current state: %s\n", state);
    }
    else
    {
        strcpy(state, args);
        printf("State updated to: %s\n", state);
    }
}

void cmd_help(char *args)
{
    printf("\nAvailable Built-in Commands:\n");
    printf("  pwd       - Display current directory\n");
    printf("  echo      - Display a message\n");
    printf("  set       - Set or display shell state\n");
    printf("  help      - Display available commands\n");
    printf("  exit      - Exit the shell\n");
}

void cmd_exit(char *args)
{
    printf("Exiting Skill 8 command dispatcher...\n");
    exit(0);
}

/* Command structure */
struct Command
{
    char name[20];
    void (*function)(char *);
};

/* Dispatch table */
struct Command dispatch_table[] =
{
    {"pwd", cmd_pwd},
    {"echo", cmd_echo},
    {"set", cmd_set},
    {"help", cmd_help},
    {"exit", cmd_exit}
};

int command_count =
    sizeof(dispatch_table) / sizeof(dispatch_table[0]);

/* Find command in dispatch table */
int find_command(char *command)
{
    for (int i = 0; i < command_count; i++)
    {
        if (strcmp(dispatch_table[i].name, command) == 0)
            return i;
    }

    return -1;
}

int main()
{
    char input[MAX_INPUT];

    printf("=====================================\n");
    printf("     SKILL 8 - COMMAND DISPATCH\n");
    printf("=====================================\n");

    printf("\nBuilt-in commands loaded: %d\n",
           command_count);

    printf("\nTesting dispatch logic...\n");

    printf("\n> help\n");
    cmd_help(NULL);

    printf("\n> pwd\n");
    cmd_pwd(NULL);

    printf("\n> echo Hello OS\n");
    cmd_echo("Hello OS");

    printf("\n> set running\n");
    cmd_set("running");

    printf("\n> set\n");
    cmd_set(NULL);

    printf("\n> unknown\n");

    if (find_command("unknown") == -1)
        printf("Error: Invalid command\n");

    printf("\n--- Interactive Command Dispatcher ---\n");
    printf("Type 'help' for commands.\n");

    while (1)
    {
        printf("\nskill8> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        char command[50];
        char args[MAX_INPUT];

        command[0] = '\0';
        args[0] = '\0';

        sscanf(input, "%49s %[^\n]",
               command, args);

        int index = find_command(command);

        if (index == -1)
        {
            printf("Error: Invalid command '%s'\n",
                   command);
        }
        else
        {
            dispatch_table[index].function(
                strlen(args) > 0 ? args : NULL
            );
        }
    }

    return 0;
}
