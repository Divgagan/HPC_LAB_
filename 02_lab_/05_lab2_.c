#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Thread %d before master\n", omp_get_thread_num());

        #pragma omp master
        {
            printf("Master thread executing this section\n");
        }

        printf("Thread %d after master\n", omp_get_thread_num());
    }
    return 0;
}



// Observations:
- Only the master thread (thread 0) executes the master block.
- Other threads do not wait and continue execution immediately after the master block.
- The execution order of threads before and after the master block is non-deterministic.
- The program demonstrates the use of the master directive in OpenMP for executing
  a block of code exclusively by the master thread without causing other threads to wait.