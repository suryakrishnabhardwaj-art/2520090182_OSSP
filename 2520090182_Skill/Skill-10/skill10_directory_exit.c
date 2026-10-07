#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char cwd[1024];

    printf("=== SKILL 10: DIRECTORY AND EXIT HANDLING ===\n");

    /* Retrieve current working directory */
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current Directory: %s\n", cwd);
    }
    else
    {
        perror("getcwd");
        return 1;
    }

    /* Display path */
    printf("Directory Path: %s\n", cwd);

    /* Save state */
    FILE *fp = fopen("skill10_state.txt", "w");

    if (fp == NULL)
    {
        perror("File open failed");
        return 1;
    }

    fprintf(fp, "Saved Directory: %s\n", cwd);
    fprintf(fp, "Program State: Active\n");

    printf("State saved successfully.\n");

    /* Cleanup resources */
    fclose(fp);
    printf("Resources cleaned up successfully.\n");

    /* Process exit request */
    printf("Process exiting successfully...\n");

    return 0;
}
