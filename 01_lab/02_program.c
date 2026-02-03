#include <stdio.h>
#include <omp.h>

int main() {
    printf("Hello Gagan, Welcome to the HPC world\n");
    int nthreads, tid;

    #pragma omp parallel
    {
        nthreads = omp_get_num_threads();
        tid = omp_get_thread_num();

        printf("Total number of threads are %d and this is thread number %d\n",
               nthreads, tid);
    }

    return 0;
}
