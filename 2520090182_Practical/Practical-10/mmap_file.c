#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    const char *filename = "mmap_data.txt";
    const char *text = "Hello from memory mapped file!\n";

    int fd = open(filename, O_RDWR | O_CREAT, 0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    size_t size = strlen(text);

    if (ftruncate(fd, size) == -1)
    {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    char *mapped = mmap(NULL, size,
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0);

    if (mapped == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    strcpy(mapped, text);

    printf("Data written using mmap(): %s", mapped);

    if (msync(mapped, size, MS_SYNC) == -1)
    {
        perror("msync");
    }

    munmap(mapped, size);
    close(fd);

    printf("File successfully written using memory mapping.\n");

    return 0;
}
