#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file;

    file = fopen("output.txt", "w");

    if (file == NULL)
    {
        perror("Error creating output.txt");
        return 1;
    }

    if (freopen("output.txt", "w", stdout) == NULL)
    {
        perror("Error redirecting stdout");
        fclose(file);
        return 1;
    }

    printf("Output redirection successful.\n");
    printf("This message is written into output.txt.\n");
    printf("File handling and stdout redirection tested successfully.\n");

    fclose(file);

    return 0;
}
