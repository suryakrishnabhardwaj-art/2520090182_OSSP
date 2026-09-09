#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/os_server_fifo"

struct request {
    int client_pid;
    char message[100];
};

int main()
{
    struct request req;
    char response[200];
    char client_fifo[100];

    // Create named pipe
    mkfifo(FIFO_NAME, 0666);

    printf("Server started...\n");
    printf("Waiting for clients...\n");

    while (1)
    {
        int fd = open(FIFO_NAME, O_RDONLY);

        read(fd, &req, sizeof(req));

        close(fd);

        printf("\nClient PID: %d\n", req.client_pid);
        printf("Message: %s\n", req.message);

        // Create response
        snprintf(response, sizeof(response),
                 "Server received your message: %s",
                 req.message);

        // Create client's private FIFO name
        snprintf(client_fifo, sizeof(client_fifo),
                 "/tmp/os_client_%d", req.client_pid);

        // Open client's FIFO
        int client_fd = open(client_fifo, O_WRONLY);

        write(client_fd, response, strlen(response) + 1);

        close(client_fd);

        printf("Response sent to Client %d\n", req.client_pid);
    }

    return 0;
}
