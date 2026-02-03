
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000

int main() {
    double *A, *B, *C;
    double start, end;

    A = (double *)malloc(N * sizeof(double));
    B = (double *)malloc(N * sizeof(double));
    C = (double *)malloc(N * sizeof(double));

    for (int i = 0; i < N; i++) {
        A[i] = i * 1.0;
        B[i] = i * 2.0;
    }

    start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }
    end = omp_get_wtime();
    printf("Sequential Time: %f seconds\n", end - start);

    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }
    end = omp_get_wtime();
    printf("Parallel Time: %f seconds\n", end - start);
}


Observations:
Here we can see that the time for the parallel execution is 0.010886 seconds
 and that for sequential execution is 0.003649 seconds so we can confirm 
parallel execution helps in reducing the time which is one of the main purpose of 
using openmp.
