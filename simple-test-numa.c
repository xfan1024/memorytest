#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define SIZE 100000000

void u64test(void *data){
    volatile uint64_t *array = data;
    for (long i = 0; i < SIZE; i++) {
        array[i] = i;
    }
    for (long i = 0; i < SIZE; i++) {
        array[i] = array[i] * 2;
    }
}

void doubletest(void *data){
    volatile double *array = data;
    for (long i = 0; i < SIZE; i++) {
        array[i] = (double)i;
    }
    for (long i = 0; i < SIZE; i++) {
        array[i] = array[i] * 2;
    }
}

void simple_test(void *data) {
    struct timespec start, end;
    long elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    u64test(data);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("[u64test]    %ld\n", elapsed);

    clock_gettime(CLOCK_MONOTONIC, &start);
    doubletest(data);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("[doubletest] %ld\n", elapsed);
}

#include <numa.h>

int main(void) {
    // get the number of nodes
    int num_nodes = numa_num_configured_nodes();
    for (int run_node = 0; run_node < num_nodes; run_node++) {

        for (int alloc_node = 0; alloc_node < num_nodes; alloc_node++) {
            numa_run_on_node(run_node);

            printf("run_node=%d, alloc_node=%d\n", run_node, alloc_node);
    
            void *data = numa_alloc_onnode(SIZE * sizeof(uint64_t), alloc_node);
            simple_test(data);
            numa_free(data, SIZE * sizeof(uint64_t));
        }
    }
    return 0;
}
