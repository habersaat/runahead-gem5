#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ITERATIONS 1000000

int main() {
    uint64_t a = 12345, b = 67890, c = 11111;
    uint64_t sum = 0;
    int i;

    printf("Compute Bound Microbenchmark (Control)\n");
    printf("Iterations: %d\n", NUM_ITERATIONS);

    printf("Starting compute loop...\n");

    for (i = 0; i < NUM_ITERATIONS; i++) {
        a = a * 1103515245 + 12345;
        b = b ^ (a >> 16);
        c = c + (a & 0xFFFF) + (b & 0xFFFF);
        sum += (a ^ b ^ c);
    }

    printf("Sum: %lu\n", sum);
    printf("Done.\n");

    return 0;
}
