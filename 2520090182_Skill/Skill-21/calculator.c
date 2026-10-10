#include <string.h>
#include "calculator.h"

int calculate(const char *operation, double a, double b, double *result)
{
    if (operation == NULL || result == NULL)
        return 0;

    if (strcmp(operation, "add") == 0) {
        *result = a + b;
    }
    else if (strcmp(operation, "subtract") == 0) {
        *result = a - b;
    }
    else if (strcmp(operation, "multiply") == 0) {
        *result = a * b;
    }
    else if (strcmp(operation, "divide") == 0) {
        if (b == 0.0)
            return 0;

        *result = a / b;
    }
    else {
        return 0;
    }

    return 1;
}
