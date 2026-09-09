#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_BUFFER 100
#define MAX_HISTORY 50

// Enable character-by-character input
void enableRawMode(struct termios *oldSettings) {
    struct termios newSettings;

    tcgetattr(STDIN_FILENO, oldSettings);

    newSettings = *oldSettings;

    // Disable canonical mode and echo
    newSettings.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
}

// Restore normal terminal mode
void disableRawMode(struct termios *oldSettings) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldSettings);
}

int main() {

    // ==============================
    // INPUT BUFFER
    // ==============================
    char buffer[MAX_BUFFER];

    // ==============================
    // COMMAND HISTORY BUFFER
    // ==============================
    char history[MAX_HISTORY][MAX_BUFFER];

    int bufferIndex;
    int historyCount = 0;

    char ch;

    struct termios oldSettings;

    printf("========================================\n");
    printf("       INTERACTIVE COMMAND SHELL\n");
    printf("========================================\n");

    while (1) {

        printf("\nmyShell> ");
        fflush(stdout);

        // Reset input buffer
        bufferIndex = 0;
        memset(buffer, 0, sizeof(buffer));

        // Enable character-by-character input
        enableRawMode(&oldSettings);

        while (1) {

            // Read one character
            read(STDIN_FILENO, &ch, 1);

            // ==================================
            // ENTER KEY
            // ==================================
            if (ch == '\n' || ch == '\r') {

                buffer[bufferIndex] = '\0';

                printf("\n");

                break;
            }

            // ==================================
            // BACKSPACE
            // ==================================
            else if (ch == 127 || ch == '\b') {

                if (bufferIndex > 0) {

                    // Remove character from buffer
                    bufferIndex--;

                    buffer[bufferIndex] = '\0';

                    // Remove character from screen
                    printf("\b \b");

                    fflush(stdout);
                }
            }

            // ==================================
            // NORMAL CHARACTER
            // ==================================
            else if (bufferIndex < MAX_BUFFER - 1) {

                // Store character in buffer
                buffer[bufferIndex] = ch;

                // Move buffer index
                bufferIndex++;

                // Display character
                putchar(ch);

                fflush(stdout);
            }
        }

        // Restore normal terminal settings
        disableRawMode(&oldSettings);

        // ==================================
        // EMPTY INPUT
        // ==================================
        if (bufferIndex == 0) {
            continue;
        }

        // ==================================
        // SAVE COMMAND TO HISTORY
        // ==================================
        if (historyCount < MAX_HISTORY) {

            strcpy(history[historyCount], buffer);

            historyCount++;
        }

        // ==================================
        // EXIT COMMAND
        // ==================================
        if (strcmp(buffer, "exit") == 0) {

            printf("Exiting program...\n");

            break;
        }

        // ==================================
        // HISTORY COMMAND
        // ==================================
        else if (strcmp(buffer, "history") == 0) {

            printf("\n========== COMMAND HISTORY ==========\n");

            // Don't display the current "history"
            // command itself
            for (int i = 0; i < historyCount - 1; i++) {

                printf("%d. %s\n",
                       i + 1,
                       history[i]);
            }

            printf("=====================================\n");
        }

        // ==================================
        // HELLO COMMAND
        // ==================================
        else if (strcmp(buffer, "hello") == 0) {

            printf("Hello! Welcome to the command shell.\n");
        }

        // ==================================
        // HELP COMMAND
        // ==================================
        else if (strcmp(buffer, "help") == 0) {

            printf("\nAvailable Commands:\n");
            printf("hello   - Display greeting\n");
            printf("help    - Display commands\n");
            printf("history - Show previous commands\n");
            printf("exit    - Exit program\n");
        }

        // ==================================
        // UNKNOWN COMMAND
        // ==================================
        else {

            printf("Unknown command: %s\n", buffer);
        }
    }

    return 0;
}
