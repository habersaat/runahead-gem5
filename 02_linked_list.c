#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_NODES (1000)
#define NUM_TRAVERSALS 50

struct Node
{
    struct Node *next;
    uint64_t data[7];  /* Pad to 64 bytes (cache line) */
};

int main() {
    struct Node *nodes;
    struct Node *head, *curr;
    uint64_t sum = 0;
    int i, t;

    printf("Linked List Traversal Microbenchmark\n");
    printf("Nodes: %d, Traversals: %d\n", NUM_NODES, NUM_TRAVERSALS);

    nodes = (struct Node *)malloc(NUM_NODES * sizeof(struct Node));
    if (!nodes) {
        printf("ERROR: malloc failed\n");
        return 1;
    }

    int *indices = (int *)malloc(NUM_NODES * sizeof(int));
    for (i = 0; i < NUM_NODES; i++) indices[i] = i;

    uint64_t seed = 12345;
    for (i = NUM_NODES - 1; i > 0; i--) {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        int j = seed % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }

    head = &nodes[indices[0]];
    for (i = 0; i < NUM_NODES - 1; i++) {
        nodes[indices[i]].next = &nodes[indices[i + 1]];
        nodes[indices[i]].data[0] = i;
    }
    nodes[indices[NUM_NODES - 1]].next = NULL;
    nodes[indices[NUM_NODES - 1]].data[0] = NUM_NODES - 1;

    printf("Starting linked list traversals...\n");

    for (t = 0; t < NUM_TRAVERSALS; t++) {
        curr = head;
        while (curr != NULL) {
            sum += curr->data[0];
            curr = curr->next;  /* Pointer chase - cache miss likely */
        }
    }

    printf("Sum: %lu\n", sum);
    printf("Done.\n");

    free(nodes);
    free(indices);
    return 0;
}
