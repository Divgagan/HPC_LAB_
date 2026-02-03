#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Thread %d before single\n", omp_get_thread_num());

        #pragma omp single
        {
            printf("Single executed by thread %d\n",
                   omp_get_thread_num());
        }

        printf("Thread %d after single\n", omp_get_thread_num());
    }
    return 0;
}

// Observations:
- Only one thread executes the single block, while others wait.
- The thread that executes the single block is non-deterministic.
- After the single block, all threads continue execution.
- The implicit barrier at the end of the single block ensures 
   synchronization before proceeding.
- Execution order of threads before and after the single block is non-deterministic.
- The program demonstrates the use of the single directive in OpenMP for executing 
   a block of code by only one thread while others wait.