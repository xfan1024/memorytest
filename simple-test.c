#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define SIZE 100000000

static volatile uint64_t data[SIZE];

void u64test(){
    volatile uint64_t *array = data;
    for (long i = 0; i < SIZE; i++) {
        array[i] = i;
    }
    for (long i = 0; i < SIZE; i++) {
        array[i] = array[i] * 2;
    }
}

void doubletest(){
    volatile double *array = (volatile double*)data;
    for (long i = 0; i < SIZE; i++) {
        array[i] = (double)i;
    }
    for (long i = 0; i < SIZE; i++) {
        array[i] = array[i] * 2;
    }
}

int main(void) {
    struct timespec start, end;
    long elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    u64test();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("[u64test]    %ld\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    doubletest();
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("[doubletest] %ld\n", elapsed);

    return 0;
}
