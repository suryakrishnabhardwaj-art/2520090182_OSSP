#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_HISTORY 10
#define MAX_INPUT 1024

char *history[MAX_HISTORY];
int history_count = 0;
int history_index = -1;

void add_history(const char *command)
{
    if (command == NULL || strlen(command) == 0)
        return;

    if (history_count == MAX_HISTORY)
    {
        free(history[0]);

        for (int i = 1; i < MAX_HISTORY; i++)
            history[i - 1] = history[i];

        history_count--;
    }

    history[history_count] = strdup(command);
    history_count++;
}

void display_buffer(const char *buffer)
{
    printf("\r\033[K> %s", buffer);
    fflush(stdout);
}

void copy_history_to_buffer(char *buffer, int index)
{
    if (index >= 0 && index < history_count)
    {
        strcpy(buffer, history[index]);
        display_buffer(buffer);
    }
}

void run_shell()
{
    char buffer[MAX_INPUT];
    int length;
    char ch;

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        length = 0;
        history_index = history_count;

        printf("\n> ");
        fflush(stdout);

        while (1)
        {
            ch = getchar();

            if (ch == '\n')
            {
                buffer[length] = '\0';

                if (strcmp(buffer, "exit") == 0)
                    return;

                if (length > 0)
                {
                    printf("\nExecuted: %s\n", buffer);
                    add_history(buffer);
                }

                break;
            }

            /* Backspace */
            if (ch == 127 || ch == 8)
            {
                if (length > 0)
                {
                    length--;
                    buffer[length] = '\0';

                    printf("\b \b");
                    fflush(stdout);
                }

                continue;
            }

            /* Escape sequence */
            if (ch == 27)
            {
                char c1 = getchar();

                if (c1 == '[')
                {
                    char c2 = getchar();

                    /* Up arrow */
                    if (c2 == 'A')
                    {
                        if (history_count > 0 && history_index > 0)
                        {
                            history_index--;
                            copy_history_to_buffer(
                                buffer,
                                history_index
                            );
                            length = strlen(buffer);
                        }
                    }

                    /* Down arrow */
                    else if (c2 == 'B')
                    {
                        if (history_index < history_count - 1)
                        {
                            history_index++;
                            copy_history_to_buffer(
                                buffer,
                                history_index
                            );
                            length = strlen(buffer);
                        }
                        else
                        {
                            history_index = history_count;
                            memset(buffer, 0, sizeof(buffer));
                            length = 0;
                            display_buffer(buffer);
                        }
                    }
                }

                continue;
            }

            /* Normal character */
            if (length < MAX_INPUT - 1)
            {
                buffer[length++] = ch;
                buffer[length] = '\0';

                putchar(ch);
                fflush(stdout);
            }
        }
    }
}

int main()
{
    printf("====================================\n");
    printf("     OSSP SKILL-03 HISTORY SHELL    \n");
    printf("====================================\n");
    printf("Type commands and use UP/DOWN arrows.\n");
    printf("Type 'exit' to quit.\n");

    run_shell();

    for (int i = 0; i < history_count; i++)
        free(history[i]);

    printf("\nShell terminated successfully.\n");

    return 0;
}
