#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE (1024 * 1024)  /* 1M entries = 8MB for 64-bit */

#define NUM_ITERATIONS 10000

#define CHAIN_DEPTH 4

int main() {
    uint64_t *arr;
    uint64_t idx, sum = 0;
    int i, j;

    printf("Dependent Load Chain Microbenchmark\n");
    printf("Array size: %d entries (%lu MB)\n",
           ARRAY_SIZE, (ARRAY_SIZE * sizeof(uint64_t)) / (1024 * 1024));
    printf("Iterations: %d, Chain depth: %d\n", NUM_ITERATIONS, CHAIN_DEPTH);

    /* Allocate array */
    arr = (uint64_t *)malloc(ARRAY_SIZE * sizeof(uint64_t));
    if (!arr) {
        printf("ERROR: malloc failed\n");
        return 1;
    }

    uint64_t seed = 12345;
    for (i = 0; i < ARRAY_SIZE; i++) {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        arr[i] = seed % ARRAY_SIZE;
    }

    printf("Starting dependent load chains...\n");

    idx = 0;
    for (i = 0; i < NUM_ITERATIONS; i++) {
        for (j = 0; j < CHAIN_DEPTH; j++) {
            idx = arr[idx];
        }
        sum += idx;

        idx = (idx + 1) % ARRAY_SIZE;
    }

    printf("Sum: %lu\n", sum);
    printf("Done.\n");

    free(arr);
    return 0;
}
