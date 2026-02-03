#include <omp.h>
#include <stdio.h>

int numt;

int main() {

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        if (tid == 0) {
            numt = omp_get_num_threads();
        }
        printf("Hello World from Thread %d of %d\n", tid, numt);
    } 
    
    // implicit barrier here

   printf("All the threads have joined back to the master thread.\n");

    return 0;
}

/**🔹 Observations (Write in Report)

All threads execute concurrently inside the parallel region.

The statement after the parallel block executes only after all threads complete.

This demonstrates the fork–join model, where join acts as an implicit barrier.

Execution order of thread prints is non-deterministic. */