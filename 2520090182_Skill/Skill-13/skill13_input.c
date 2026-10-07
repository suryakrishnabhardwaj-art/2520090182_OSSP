#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file;

    file = fopen("input.txt", "r");

    if (file == NULL)
    {
        perror("Error opening input.txt");
        return 1;
    }

    if (freopen("input.txt", "r", stdin) == NULL)
    {
        perror("Error redirecting stdin");
        fclose(file);
        return 1;
    }

    printf("Input redirected successfully.\n");
    printf("Contents of input.txt:\n");

    char ch;

    while ((ch = getchar()) != EOF)
    {
        putchar(ch);
    }

    fclose(file);

    return 0;
}
