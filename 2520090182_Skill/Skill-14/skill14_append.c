#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    const char *file = "append_data.txt";

    // Open file in append mode
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    const char *message = "New data appended to the file.\n";

    if (write(fd, message, strlen(message)) == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }

    close(fd);

    printf("Data appended successfully.\n");

    printf("\nCurrent file contents:\n");

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        perror("Error reading file");
        return 1;
    }

    char buffer[100];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    fclose(fp);

    return 0;
}
