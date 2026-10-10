#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "calculator.h"
#include "logger.h"

static int parse_number(const char *text, double *number)
{
    char *end;
    errno = 0;

    double value = strtod(text, &end);

    if (text == end || *end != '\0' ||
        errno == ERANGE || !isfinite(value)) {
        return 0;
    }

    *number = value;
    return 1;
}

int main(int argc, char *argv[])
{
    double a, b, result;

    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s <add|subtract|multiply|divide> <number1> <number2>\n",
                argv[0]);
        log_error("Invalid command syntax or incorrect number of arguments.");
        return 1;
    }

    if (!parse_number(argv[2], &a) ||
        !parse_number(argv[3], &b)) {
        fprintf(stderr, "Error: Invalid numeric input.\n");
        log_error("Invalid numeric input detected.");
        return 1;
    }

    if (!calculate(argv[1], a, b, &result)) {
        if (strcmp(argv[1], "divide") == 0 && b == 0.0) {
            fprintf(stderr, "Error: Division by zero is not allowed.\n");
            log_error("Division by zero attempted.");
        } else {
            fprintf(stderr, "Error: Unsupported operation '%s'.\n",
                    argv[1]);
            log_error("Unsupported calculator operation.");
        }

        return 1;
    }

    printf("Operation: %s\n", argv[1]);
    printf("Input values: %.2f and %.2f\n", a, b);
    printf("Result: %.2f\n", result);
    printf("Execution completed successfully.\n");

    return 0;
}
