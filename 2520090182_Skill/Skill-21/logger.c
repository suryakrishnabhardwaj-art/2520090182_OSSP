#include <stdio.h>
#include <time.h>
#include "logger.h"

void log_error(const char *message)
{
    FILE *file = fopen("error.log", "a");

    if (file == NULL) {
        perror("Unable to open error log");
        return;
    }

    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    char timestamp[32] = "unknown-time";

    if (time_info != NULL) {
        if (strftime(timestamp, sizeof(timestamp),
                     "%Y-%m-%d %H:%M:%S", time_info) == 0) {
            snprintf(timestamp, sizeof(timestamp), "unknown-time");
        }
    }

    fprintf(file, "[%s] ERROR: %s\n", timestamp, message);
    fclose(file);
}
