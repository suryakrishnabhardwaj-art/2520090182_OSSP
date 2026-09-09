#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/os_server_fifo"

struct request {
    int client_pid;
    char message[100];
};

int main()
{
    struct request req;
    char client_fifo[100];
    char response[200];

    // Get client PID
    req.client_pid = getpid();

    // Create private FIFO for this client
    snprintf(client_fifo, sizeof(client_fifo),
             "/tmp/os_client_%d", req.client_pid);

    mkfifo(client_fifo, 0666);

    // Get message from user
    printf("Enter message: ");
    fgets(req.message, sizeof(req.message), stdin);

    // Remove newline
    req.message[strcspn(req.message, "\n")] = '\0';

    // Open server FIFO
    int fd = open(SERVER_FIFO, O_WRONLY);

    // Send request
    write(fd, &req, sizeof(req));

    close(fd);

    printf("Message sent to server.\n");

    // Wait for response
    int client_fd = open(client_fifo, O_RDONLY);

    read(client_fd, response, sizeof(response));

    close(client_fd);

    printf("Server response: %s\n", response);

    // Remove private FIFO
    unlink(client_fifo);

    return 0;
}
