#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Linked list node */
struct Node {
    int data;
    struct Node *next;
};

int main() {

    printf("===== DYNAMIC MEMORY AND LINKED LIST DEMO =====\n\n");

    /* Dynamic array */
    int capacity = 3;
    int size = 0;

    int *buffer = malloc(capacity * sizeof(int));

    if (buffer == NULL) {
        printf("Initial memory allocation failed!\n");
        return 1;
    }

    printf("Initial buffer capacity: %d\n", capacity);

    /* Add elements */
    for (int i = 0; i < 6; i++) {

        /* Resize before buffer becomes full */
        if (size == capacity) {
            capacity *= 2;

            int *temp = realloc(buffer,
                                capacity * sizeof(int));

            if (temp == NULL) {
                printf("Buffer resizing failed!\n");
                free(buffer);
                return 1;
            }

            buffer = temp;

            printf("Buffer resized to capacity: %d\n", capacity);
        }

        buffer[size] = (i + 1) * 10;
        size++;
    }

    printf("\nDynamic buffer contents:\n");

    for (int i = 0; i < size; i++) {
        printf("%d ", buffer[i]);
    }

    printf("\n");

    /*
     * Linked list
     */

    struct Node *head = NULL;
    struct Node *newNode;

    for (int i = 1; i <= 3; i++) {

        newNode = malloc(sizeof(struct Node));

        if (newNode == NULL) {
            printf("Linked list allocation failed!\n");
            free(buffer);
            return 1;
        }

        newNode->data = i * 100;
        newNode->next = head;
        head = newNode;
    }

    printf("\nLinked list contents:\n");

    struct Node *current = head;

    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }

    printf("NULL\n");

    /*
     * Free linked list
     */

    current = head;

    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }

    /* Free dynamic array */
    free(buffer);

    printf("\nDynamic array memory released.\n");
    printf("Linked list memory released.\n");
    printf("Memory management completed successfully.\n");

    return 0;
}
