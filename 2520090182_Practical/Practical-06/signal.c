#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_received = 0;

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nSIGINT received!\n");
        signal_received = 1;
    }
    else if (sig == SIGTERM)
    {
        printf("\nSIGTERM received!\n");
        signal_received = 1;
    }
    else if (sig == SIGUSR1)
    {
        printf("\nSIGUSR1 received!\n");
    }
}

int main()
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGUSR1, handle_signal);

    printf("Signal handling program started.\n");
    printf("Process ID: %d\n", getpid());
    printf("Waiting for signals...\n");

    while (!signal_received)
    {
        pause();
    }

    printf("Program terminated gracefully.\n");

    return 0;
}
