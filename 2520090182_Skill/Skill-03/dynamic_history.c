#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *command;
    struct Node *next;
} Node;

typedef struct
{
    char **commands;
    size_t size;
    size_t capacity;
} History;

void init_history(History *history)
{
    history->size = 0;
    history->capacity = 2;

    history->commands =
        malloc(history->capacity * sizeof(char *));

    if (history->commands == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
}

void resize_history(History *history)
{
    history->capacity *= 2;

    char **temp =
        realloc(history->commands,
                history->capacity * sizeof(char *));

    if (temp == NULL)
    {
        perror("realloc");
        free(history->commands);
        exit(EXIT_FAILURE);
    }

    history->commands = temp;
}

void add_command(History *history, const char *command)
{
    if (history->size == history->capacity)
        resize_history(history);

    history->commands[history->size] =
        malloc(strlen(command) + 1);

    if (history->commands[history->size] == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(history->commands[history->size], command);
    history->size++;
}

void display_history(const History *history)
{
    printf("\nCommand History:\n");

    for (size_t i = 0; i < history->size; i++)
    {
        printf("%zu -> %s\n",
               i + 1,
               history->commands[i]);
    }
}

void free_history(History *history)
{
    for (size_t i = 0; i < history->size; i++)
        free(history->commands[i]);

    free(history->commands);

    history->commands = NULL;
    history->size = 0;
    history->capacity = 0;
}

Node *create_node(const char *command)
{
    Node *node = malloc(sizeof(Node));

    if (node == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    node->command = malloc(strlen(command) + 1);

    if (node->command == NULL)
    {
        perror("malloc");
        free(node);
        exit(EXIT_FAILURE);
    }

    strcpy(node->command, command);
    node->next = NULL;

    return node;
}

void free_list(Node *head)
{
    while (head != NULL)
    {
        Node *temp = head;
        head = head->next;

        free(temp->command);
        free(temp);
    }
}

int main()
{
    History history;

    init_history(&history);

    add_command(&history, "ls");
    add_command(&history, "pwd");
    add_command(&history, "gcc program.c");
    add_command(&history, "./program");

    display_history(&history);

    printf("\nDynamic capacity: %zu\n",
           history.capacity);

    Node *head = create_node("first command");
    head->next = create_node("second command");
    head->next->next = create_node("third command");

    printf("\nLinked List:\n");

    Node *current = head;

    while (current != NULL)
    {
        printf("%s\n", current->command);
        current = current->next;
    }

    free_list(head);
    free_history(&history);

    printf("\nAll dynamically allocated memory released.\n");

    return 0;
}
