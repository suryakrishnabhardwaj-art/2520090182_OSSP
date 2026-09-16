#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;

    int *a = (int *)malloc(3 * sizeof(int));

    for (i = 0; i < 3; i++)
        a[i] = (i + 1) * 10;

    printf("malloc: ");
    for (i = 0; i < 3; i++)
        printf("%d ", a[i]);
    printf("\n");

    int *b = (int *)calloc(3, sizeof(int));

    printf("calloc: ");
    for (i = 0; i < 3; i++)
        printf("%d ", b[i]);
    printf("\n");

    a = (int *)realloc(a, 5 * sizeof(int));
    a[3] = 40;
    a[4] = 50;

    printf("realloc: ");
    for (i = 0; i < 5; i++)
        printf("%d ", a[i]);
    printf("\n");

    free(a);
    free(b);

    printf("Memory freed successfully.\n");

    return 0;
}
