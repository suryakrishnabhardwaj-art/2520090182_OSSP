#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;

    printf("Testing stderr redirection...\n");

    fd = open("file_that_does_not_exist.txt", O_RDONLY);

    if (fd == -1) {
        perror("ERROR");
        return 1;
    }

    close(fd);

    return 0;
}
