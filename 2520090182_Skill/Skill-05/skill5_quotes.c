#include <stdio.h>
#include <string.h>

int main() {
    char name[] = "Bhardwaj";
    char text[] = "Operating Systems";

    printf("==========================================\n");
    printf("       SKILL-05: QUOTING IN SHELL\n");
    printf("==========================================\n\n");

    /* Single Quotes */
    printf("1. SINGLE QUOTES\n");
    printf("Single quotes preserve literal content:\n");
    printf("'Hello $name'\n\n");

    /* Double Quotes */
    printf("2. DOUBLE QUOTES\n");
    printf("Double quotes allow variable expansion:\n");
    printf("\"Hello %s\"\n\n", name);

    /* Literal Content */
    printf("3. PRESERVE LITERAL CONTENT\n");
    printf("'This is $name and * is literal content.'\n\n");

    /* Variable Expansion */
    printf("4. VARIABLE EXPANSION\n");
    printf("Variable name = %s\n", name);
    printf("Variable text = %s\n\n", text);

    /* Preserve Spaces */
    printf("5. PRESERVE SPACES\n");
    printf("\"Operating Systems Skill Five\"\n\n");

    /* Nested Tokens */
    printf("6. NESTED TOKENS\n");
    printf("Outer token: Name is %s\n", name);
    printf("Inner token: Name is $name\n\n");

    /* Parsing */
    printf("7. PARSING QUOTED STRINGS\n");
    char first[] = "Operating";
    char second[] = "Systems";
    char combined[50];

    strcpy(combined, first);
    strcat(combined, " ");
    strcat(combined, second);

    printf("Combined string: %s\n\n", combined);

    /* Quoted Commands */
    printf("8. QUOTED COMMANDS\n");
    char command[] = "echo Hello from quoted command";
    printf("Command stored as string: %s\n", command);
    printf("Command parsing is handled by the shell.\n\n");

    /* Edge Cases */
    printf("9. EDGE CASES\n");
    char empty[] = "";
    printf("Empty string length = %lu\n", strlen(empty));

    printf("Special characters: $ @ # * ? !\n\n");

    /* Comparison */
    printf("10. COMPARISON\n");
    printf("Single quotes : Preserve literal content\n");
    printf("Double quotes : Allow variable expansion\n");

    printf("\n==========================================\n");
    printf("        SKILL-05 COMPLETED\n");
    printf("==========================================\n");

    return 0;
}
