#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 5
#define MAX_CMD_LEN 100

char history[MAX_HISTORY][MAX_CMD_LEN];
int history_count = 0;

void add_command(char *cmd)
{
    if (history_count < MAX_HISTORY)
    {
        strcpy(history[history_count], cmd);
        history_count++;
    }
    else
    {
        for (int i = 1; i < MAX_HISTORY; i++)
        {
            strcpy(history[i - 1], history[i]);
        }

        strcpy(history[MAX_HISTORY - 1], cmd);
    }
}

void display_history()
{
    printf("\nCommand History:\n");

    for (int i = 0; i < history_count; i++)
    {
        printf("%d. %s\n", i + 1, history[i]);
    }
}

void retrieve_command(int index)
{
    if (index >= 1 && index <= history_count)
    {
        printf("\nRetrieved command: %s\n", history[index - 1]);
    }
    else
    {
        printf("\nInvalid history entry!\n");
    }
}

void validate_history()
{
    if (history_count <= MAX_HISTORY)
        printf("\nHistory validation: CONSISTENT\n");
    else
        printf("\nHistory validation: INCONSISTENT\n");
}

int main()
{
    printf("OS Skill-11: Command History Management\n");

    add_command("ls");
    add_command("pwd");
    add_command("gcc program.c");
    add_command("./program");
    add_command("git status");
    add_command("whoami");

    display_history();

    retrieve_command(3);

    validate_history();

    printf("\nHistory capacity: %d commands\n", MAX_HISTORY);

    return 0;
}
