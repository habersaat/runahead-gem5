#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE (512 * 1024)  /* 512K entries = 4MB */
#define NUM_ITERATIONS 1000
#define NUM_STREAMS 4  /* Independent load streams */

int main() {
    uint64_t *arr;
    uint64_t idx[NUM_STREAMS];
    uint64_t sum = 0;
    int i, s;

    printf("Independent Loads Microbenchmark\n");
    printf("Array size: %d entries (%lu MB)\n",
           ARRAY_SIZE, (ARRAY_SIZE * sizeof(uint64_t)) / (1024 * 1024));
    printf("Iterations: %d, Streams: %d\n", NUM_ITERATIONS, NUM_STREAMS);

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

    for (s = 0; s < NUM_STREAMS; s++) {
        idx[s] = (ARRAY_SIZE / NUM_STREAMS) * s;
    }

    printf("Starting independent load streams...\n");

    for (i = 0; i < NUM_ITERATIONS; i++) {
        uint64_t v0 = arr[idx[0]];
        uint64_t v1 = arr[idx[1]];
        uint64_t v2 = arr[idx[2]];
        uint64_t v3 = arr[idx[3]];

        idx[0] = v0;
        idx[1] = v1;
        idx[2] = v2;
        idx[3] = v3;

        sum += v0 + v1 + v2 + v3;
    }

    printf("Sum: %lu\n", sum);
    printf("Done.\n");

    free(arr);
    return 0;
}
