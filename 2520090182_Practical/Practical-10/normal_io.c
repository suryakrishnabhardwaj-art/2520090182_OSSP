#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    const char *filename = "normal_io.txt";
    const char *text = "Hello from normal read/write operations!\n";

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd, text, strlen(text));
    close(fd);

    fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    char buffer[100] = {0};

    read(fd, buffer, sizeof(buffer) - 1);

    printf("Data read using normal read(): %s", buffer);

    close(fd);

    return 0;
}
