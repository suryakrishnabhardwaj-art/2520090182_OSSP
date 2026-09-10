#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TEXT 100
#define MAX_ARGS 50

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_INPUT,
    TOKEN_OUTPUT,
    TOKEN_APPEND,
    TOKEN_BACKGROUND,
    TOKEN_SEMICOLON
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TEXT];
} Token;

typedef struct {
    char command[MAX_TEXT];
    char *args[MAX_ARGS];
    int arg_count;

    char input_file[MAX_TEXT];
    char output_file[MAX_TEXT];

    int append;
    int background;
} Command;

const char *tokenTypeName(TokenType type)
{
    switch (type) {
        case TOKEN_WORD:       return "WORD";
        case TOKEN_PIPE:       return "PIPE";
        case TOKEN_INPUT:      return "INPUT";
        case TOKEN_OUTPUT:     return "OUTPUT";
        case TOKEN_APPEND:     return "APPEND";
        case TOKEN_BACKGROUND: return "BACKGROUND";
        case TOKEN_SEMICOLON:  return "SEMICOLON";
        default:               return "UNKNOWN";
    }
}

/* ---------------------------------------------------------
   TOKENIZER
   --------------------------------------------------------- */

int tokenize(const char *input, Token tokens[])
{
    int count = 0;
    int i = 0;

    while (input[i] != '\0') {

        /* Handle whitespace */
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|') {
            tokens[count].type = TOKEN_PIPE;
            strcpy(tokens[count].value, "|");
            count++;
            i++;
            continue;
        }

        /* Input redirection */
        if (input[i] == '<') {
            tokens[count].type = TOKEN_INPUT;
            strcpy(tokens[count].value, "<");
            count++;
            i++;
            continue;
        }

        /* Output / Append redirection */
        if (input[i] == '>') {

            if (input[i + 1] == '>') {
                tokens[count].type = TOKEN_APPEND;
                strcpy(tokens[count].value, ">>");
                count++;
                i += 2;
            } else {
                tokens[count].type = TOKEN_OUTPUT;
                strcpy(tokens[count].value, ">");
                count++;
                i++;
            }

            continue;
        }

        /* Background */
        if (input[i] == '&') {
            tokens[count].type = TOKEN_BACKGROUND;
            strcpy(tokens[count].value, "&");
            count++;
            i++;
            continue;
        }

        /* Semicolon */
        if (input[i] == ';') {
            tokens[count].type = TOKEN_SEMICOLON;
            strcpy(tokens[count].value, ";");
            count++;
            i++;
            continue;
        }

        /* Word */
        if (count >= MAX_TOKENS) {
            printf("ERROR: Too many tokens.\n");
            return -1;
        }

        int j = 0;

        while (input[i] != '\0' &&
               !isspace((unsigned char)input[i]) &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>' &&
               input[i] != '&' &&
               input[i] != ';') {

            tokens[count].value[j++] = input[i++];
        }

        tokens[count].value[j] = '\0';
        tokens[count].type = TOKEN_WORD;
        count++;
    }

    return count;
}

/* ---------------------------------------------------------
   TOKEN DEBUG OUTPUT
   --------------------------------------------------------- */

void printTokens(Token tokens[], int count)
{
    printf("\n========== TOKEN STREAM ==========\n");

    for (int i = 0; i < count; i++) {
        printf("Token %d : %-12s Value = \"%s\"\n",
               i + 1,
               tokenTypeName(tokens[i].type),
               tokens[i].value);
    }

    printf("==================================\n");
}

/* ---------------------------------------------------------
   SYNTAX VALIDATION
   --------------------------------------------------------- */

int validateTokens(Token tokens[], int count)
{
    if (count == 0) {
        printf("\nERROR: Empty command.\n");
        return 0;
    }

    if (tokens[0].type == TOKEN_PIPE) {
        printf("\nERROR: Command cannot start with a pipe.\n");
        return 0;
    }

    if (tokens[count - 1].type == TOKEN_PIPE) {
        printf("\nERROR: Command cannot end with a pipe.\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {

        /* Detect consecutive pipes */
        if (tokens[i].type == TOKEN_PIPE &&
            i + 1 < count &&
            tokens[i + 1].type == TOKEN_PIPE) {

            printf("\nERROR: Empty command between pipes.\n");
            return 0;
        }

        /* Redirection needs a filename */
        if (tokens[i].type == TOKEN_INPUT ||
            tokens[i].type == TOKEN_OUTPUT ||
            tokens[i].type == TOKEN_APPEND) {

            if (i + 1 >= count ||
                tokens[i + 1].type != TOKEN_WORD) {

                printf("\nERROR: Redirection requires a file name.\n");
                return 0;
            }
        }

        /* Background should be at the end */
        if (tokens[i].type == TOKEN_BACKGROUND &&
            i != count - 1) {

            printf("\nERROR: '&' must appear at the end.\n");
            return 0;
        }
    }

    printf("\nSYNTAX VALIDATION: SUCCESS\n");
    return 1;
}

/* ---------------------------------------------------------
   PARSE COMMANDS
   --------------------------------------------------------- */

int parseCommands(Token tokens[], int count, Command commands[])
{
    int commandCount = 0;
    int i = 0;

    while (i < count) {

        Command *cmd = &commands[commandCount];

        memset(cmd, 0, sizeof(Command));

        /* Parse command name */
        if (tokens[i].type == TOKEN_WORD) {

            strcpy(cmd->command, tokens[i].value);

            cmd->args[cmd->arg_count] = strdup(tokens[i].value);
            cmd->arg_count++;

            i++;
        } else {
            return -1;
        }

        /* Parse command contents */
        while (i < count &&
               tokens[i].type != TOKEN_PIPE &&
               tokens[i].type != TOKEN_SEMICOLON) {

            if (tokens[i].type == TOKEN_WORD) {

                if (cmd->arg_count < MAX_ARGS) {
                    cmd->args[cmd->arg_count] =
                        strdup(tokens[i].value);

                    cmd->arg_count++;
                }

                i++;
            }

            else if (tokens[i].type == TOKEN_INPUT) {

                if (i + 1 < count &&
                    tokens[i + 1].type == TOKEN_WORD) {

                    strcpy(cmd->input_file,
                           tokens[i + 1].value);

                    i += 2;
                }
            }

            else if (tokens[i].type == TOKEN_OUTPUT) {

                if (i + 1 < count &&
                    tokens[i + 1].type == TOKEN_WORD) {

                    strcpy(cmd->output_file,
                           tokens[i + 1].value);

                    cmd->append = 0;

                    i += 2;
                }
            }

            else if (tokens[i].type == TOKEN_APPEND) {

                if (i + 1 < count &&
                    tokens[i + 1].type == TOKEN_WORD) {

                    strcpy(cmd->output_file,
                           tokens[i + 1].value);

                    cmd->append = 1;

                    i += 2;
                }
            }

            else if (tokens[i].type == TOKEN_BACKGROUND) {

                cmd->background = 1;
                i++;
            }

            else {
                i++;
            }
        }

        commandCount++;

        if (i < count && tokens[i].type == TOKEN_PIPE) {
            i++;
        }

        if (i < count && tokens[i].type == TOKEN_SEMICOLON) {
            i++;
        }
    }

    return commandCount;
}

/* ---------------------------------------------------------
   EXECUTION STRUCTURE
   --------------------------------------------------------- */

void printExecutionStructure(Command commands[], int count)
{
    printf("\n======= EXECUTION STRUCTURE =======\n");

    for (int i = 0; i < count; i++) {

        printf("\nCommand %d\n", i + 1);
        printf("  Program       : %s\n", commands[i].command);

        printf("  Arguments     : ");

        for (int j = 0; j < commands[i].arg_count; j++) {
            printf("[%s] ", commands[i].args[j]);
        }

        printf("\n");

        if (strlen(commands[i].input_file) > 0)
            printf("  Input File    : %s\n",
                   commands[i].input_file);

        if (strlen(commands[i].output_file) > 0) {
            printf("  Output File   : %s\n",
                   commands[i].output_file);

            if (commands[i].append)
                printf("  Output Mode   : APPEND\n");
            else
                printf("  Output Mode   : OVERWRITE\n");
        }

        printf("  Background    : %s\n",
               commands[i].background ? "YES" : "NO");
    }

    printf("\n===================================\n");
}

/* ---------------------------------------------------------
   PARSE TREE
   --------------------------------------------------------- */

void printParseTree(Command commands[], int count)
{
    printf("\n=========== PARSE TREE =============\n");

    printf("COMMAND_LINE\n");

    for (int i = 0; i < count; i++) {

        printf("|\n");
        printf("+-- COMMAND\n");
        printf("    |\n");
        printf("    +-- PROGRAM: %s\n",
               commands[i].command);

        if (commands[i].arg_count > 1) {

            printf("    |\n");
            printf("    +-- ARGUMENTS\n");

            for (int j = 1;
                 j < commands[i].arg_count;
                 j++) {

                printf("        +-- %s\n",
                       commands[i].args[j]);
            }
        }

        if (strlen(commands[i].input_file) > 0) {
            printf("    |\n");
            printf("    +-- INPUT: %s\n",
                   commands[i].input_file);
        }

        if (strlen(commands[i].output_file) > 0) {
            printf("    |\n");
            printf("    +-- OUTPUT: %s\n",
                   commands[i].output_file);
        }
    }

    printf("====================================\n");
}

/* ---------------------------------------------------------
   MAIN
   --------------------------------------------------------- */

int main()
{
    char input[500];

    Token tokens[MAX_TOKENS];
    Command commands[MAX_TOKENS];

    printf("============================================\n");
    printf("        SKILL-04 TOKENIZER & PARSER\n");
    printf("============================================\n");

    printf("\nEnter a command:\n> ");

    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    /* Tokenization */
    int tokenCount = tokenize(input, tokens);

    if (tokenCount < 0)
        return 1;

    printTokens(tokens, tokenCount);

    /* Syntax validation */
    if (!validateTokens(tokens, tokenCount)) {
        printf("\nPARSING FAILED.\n");
        return 1;
    }

    /* Parsing */
    int commandCount =
        parseCommands(tokens, tokenCount, commands);

    if (commandCount < 0) {
        printf("\nPARSING FAILED.\n");
        return 1;
    }

    printf("\nPARSING SUCCESSFUL.\n");

    /* Parse tree */
    printParseTree(commands, commandCount);

    /* Execution structure */
    printExecutionStructure(commands, commandCount);

    /* Free allocated memory */
    for (int i = 0; i < commandCount; i++) {
        for (int j = 0; j < commands[i].arg_count; j++) {
            free(commands[i].args[j]);
        }
    }

    return 0;
}
