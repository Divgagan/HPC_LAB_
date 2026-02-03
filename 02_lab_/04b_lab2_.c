#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Thread %d before single\n", omp_get_thread_num());

        #pragma omp single nowait
        {
            printf("Single executed by thread %d\n",
                   omp_get_thread_num());
        }

        printf("Thread %d after single\n", omp_get_thread_num());
    }
    return 0;
}
