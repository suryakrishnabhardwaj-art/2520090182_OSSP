#include <stdio.h>
#include <string.h>

int main() {
    char input[200];

    printf("Enter a command or text: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    printf("\n--- Parser Output ---\n");
    printf("Original Input : %s\n", input);

    printf("Escaped Space  : ");
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ')
            printf("\\ ");
        else
            printf("%c", input[i]);
    }

    printf("\nSpecial Symbols: \$ @ # & ! * ?\n");
    printf("Escaped Quote  : \"Hello World\"\n");
    printf("Escaped Backslash: C:\\\\OS\\\\Skill6\n");

    return 0;
}
