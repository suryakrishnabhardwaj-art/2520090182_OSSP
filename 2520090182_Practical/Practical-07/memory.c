#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 100;
int global_uninit;

static int static_global = 200;

void sample_function()
{
    printf("Code address              : %p\n", (void *)sample_function);
}

int main()
{
    int stack_var = 10;
    static int static_local = 20;

    int *heap_var = (int *)malloc(sizeof(int));
    *heap_var = 30;

    printf("Process ID                : %d\n", getpid());

    sample_function();

    printf("Global variable address   : %p\n", (void *)&global_var);
    printf("Global BSS address        : %p\n", (void *)&global_uninit);
    printf("Static global address     : %p\n", (void *)&static_global);
    printf("Static local address      : %p\n", (void *)&static_local);
    printf("Heap variable address     : %p\n", (void *)heap_var);
    printf("Stack variable address    : %p\n", (void *)&stack_var);

    printf("\nProcess is running...\n");
    printf("Use /proc/%d/maps to inspect memory mappings.\n", getpid());

    sleep(120);

    free(heap_var);

    return 0;
}
