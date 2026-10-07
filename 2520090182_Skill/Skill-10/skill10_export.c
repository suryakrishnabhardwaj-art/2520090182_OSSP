#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

int valid_name(const char *name)
{
    int i;

    if (name[0] == '\0')
        return 0;

    if (!isalpha(name[0]) && name[0] != '_')
        return 0;

    for (i = 1; name[i] != '\0'; i++)
    {
        if (!isalnum(name[i]) && name[i] != '_')
            return 0;
    }

    return 1;
}

int main()
{
    char input[200];
    char *name;
    char *value;
    char *equal;
    pid_t pid;

    printf("=== SKILL 10: EXPORT ENVIRONMENT VARIABLE ===\n");

    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    /* Check export syntax */
    if (strncmp(input, "export ", 7) != 0)
    {
        printf("Error: Invalid export syntax.\n");
        return 1;
    }

    /* Find '=' */
    equal = strchr(input + 7, '=');

    if (equal == NULL)
    {
        printf("Error: Expected NAME=value format.\n");
        return 1;
    }

    /* Separate name and value */
    *equal = '\0';

    name = input + 7;
    value = equal + 1;

    /* Validate variable name */
    if (!valid_name(name))
    {
        printf("Error: Invalid variable name '%s'\n", name);
        return 1;
    }

    printf("Variable Name: %s\n", name);
    printf("Variable Value: %s\n", value);

    /* Check existing variable */
    if (getenv(name) != NULL)
    {
        printf("Existing variable found. Updating value...\n");
    }
    else
    {
        printf("New variable. Creating...\n");
    }

    /* Update environment */
    if (setenv(name, value, 1) != 0)
    {
        perror("setenv");
        return 1;
    }

    printf("Environment variable updated successfully.\n");
    printf("Current Value: %s\n", getenv(name));

    /* Create child process */
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("\n--- Child Process ---\n");
        printf("Child received %s=%s\n", name, getenv(name));

        printf("Testing variable using printenv:\n");

        execlp("printenv", "printenv", name, NULL);

        perror("execlp");
        exit(1);
    }
    else
    {
        wait(NULL);
        printf("\nParent process: Child completed successfully.\n");
    }

    return 0;
}
