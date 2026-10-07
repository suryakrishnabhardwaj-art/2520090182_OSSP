#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VARS 20
#define MAX_LEN 200

struct Variable {
    char name[50];
    char value[MAX_LEN];
};

struct Variable vars[MAX_VARS];
int var_count = 0;

/* Find variable */
int find_variable(char *name)
{
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name, name) == 0)
            return i;
    }

    return -1;
}

/* Set or update variable */
void set_variable(char *name, char *value)
{
    int index = find_variable(name);

    if (index == -1)
    {
        strcpy(vars[var_count].name, name);
        strcpy(vars[var_count].value, value);
        var_count++;
    }
    else
    {
        strcpy(vars[index].value, value);
    }
}

/* Get variable value */
char *get_variable(char *name)
{
    int index = find_variable(name);

    if (index == -1)
        return NULL;

    return vars[index].value;
}

/* Expand variables recursively */
void expand_text(char *input, char *output, int depth)
{
    if (depth > 5)
    {
        strcpy(output, "[MAX NESTING]");
        return;
    }

    char temp[MAX_LEN] = "";
    int i = 0;

    while (input[i] != '\0')
    {
        if (input[i] == '$')
        {
            i++;

            char name[50];
            int j = 0;

            while (input[i] != '\0' &&
                   ((input[i] >= 'A' && input[i] <= 'Z') ||
                    (input[i] >= 'a' && input[i] <= 'z') ||
                    (input[i] >= '0' && input[i] <= '9') ||
                    input[i] == '_'))
            {
                name[j++] = input[i++];
            }

            name[j] = '\0';

            if (j == 0)
            {
                strcat(temp, "$");
                continue;
            }

            char *value = get_variable(name);

            if (value == NULL)
            {
                strcat(temp, "[UNDEFINED]");
            }
            else
            {
                char expanded[MAX_LEN];
                expand_text(value, expanded, depth + 1);
                strcat(temp, expanded);
            }
        }
        else
        {
            int len = strlen(temp);
            temp[len] = input[i];
            temp[len + 1] = '\0';
            i++;
        }
    }

    strcpy(output, temp);
}

/* Display all variables */
void show_variables()
{
    printf("\nCurrent Variables:\n");

    for (int i = 0; i < var_count; i++)
    {
        printf("%s = %s\n",
               vars[i].name,
               vars[i].value);
    }
}

int main()
{
    char input[MAX_LEN];
    char output[MAX_LEN];

    printf("=====================================\n");
    printf("     SKILL 8 - VARIABLE EXPANSION\n");
    printf("=====================================\n");

    printf("\nSetting variables...\n");

    set_variable("NAME", "Bhardwaj");
    set_variable("CITY", "Hyderabad");
    set_variable("GREETING", "Hello $NAME");
    set_variable("MESSAGE", "$GREETING from $CITY");

    show_variables();

    printf("\n--- Expansion Tests ---\n");

    strcpy(input, "$NAME");
    expand_text(input, output, 0);
    printf("Input : %s\n", input);
    printf("Output: %s\n\n", output);

    strcpy(input, "$GREETING");
    expand_text(input, output, 0);
    printf("Input : %s\n", input);
    printf("Output: %s\n\n", output);

    strcpy(input, "$MESSAGE");
    expand_text(input, output, 0);
    printf("Input : %s\n", input);
    printf("Output: %s\n\n", output);

    strcpy(input, "Welcome $NAME to $CITY");
    expand_text(input, output, 0);
    printf("Input : %s\n", input);
    printf("Output: %s\n\n", output);

    strcpy(input, "Value = $UNKNOWN");
    expand_text(input, output, 0);
    printf("Input : %s\n", input);
    printf("Output: %s\n\n", output);

    printf("--- Updating Variable ---\n");

    set_variable("NAME", "Surya");
    strcpy(input, "$GREETING");
    expand_text(input, output, 0);

    printf("Updated NAME = Surya\n");
    printf("Input : %s\n", input);
    printf("Output: %s\n", output);

    printf("\n=====================================\n");
    printf("Variable expansion completed.\n");
    printf("=====================================\n");

    return 0;
}
