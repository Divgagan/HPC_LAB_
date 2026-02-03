#include <stdio.h>
#include <omp.h>

int counter = 0;
#pragma omp threadprivate(counter)

int main() {

    #pragma omp parallel
    {
        counter = omp_get_thread_num();
        printf("Thread %d counter value: %d\n",
               omp_get_thread_num(), counter);
    } // implicit barrier

    #pragma omp parallel
    {
        printf("Thread %d retained counter value: %d\n",
               omp_get_thread_num(), counter);
    }

    return 0;
}

/*


-->*Observation 

- Each thread has its own copy of the variable.

- Values written by threads are remembered for later use.

- The program shows how OpenMP keeps thread data safe.

- Implicit barrier ensures correct and ordered execution.



*/
