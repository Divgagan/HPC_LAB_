#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000   // Large vector size

int main() {
    double *A, *B;
    double dot = 0.0;

    A = (double*)malloc(N * sizeof(double));
    B = (double*)malloc(N * sizeof(double));

    for (int i = 0; i < N; i++) {
        A[i] = 1.0;
        B[i] = 1.0;
    }

    /* ================= STATIC SCHEDULING ================= */
    dot = 0.0;
    double start_static = omp_get_wtime();

    #pragma omp parallel for reduction(+:dot) schedule(static)
    for (int i = 0; i < N; i++) {

        // Artificial load imbalance
        if (i % 1000 == 0) {
            for (int k = 0; k < 1000; k++)
                dot += A[i] * B[i];
        } else {
            dot += A[i] * B[i];
        }
    }

    double end_static = omp_get_wtime();

    printf("Static Scheduling:\n");
    printf("Dot Product = %f\n", dot);
    printf("Time Taken = %f seconds\n\n", end_static - start_static);

    /* ================= DYNAMIC SCHEDULING ================= */
    dot = 0.0;
    double start_dynamic = omp_get_wtime();

    #pragma omp parallel for reduction(+:dot) schedule(dynamic, 100)
    for (int i = 0; i < N; i++) {

        if (i % 1000 == 0) {
            for (int k = 0; k < 1000; k++)
                dot += A[i] * B[i];
        } else {
            dot += A[i] * B[i];
        }
    }

    double end_dynamic = omp_get_wtime();

    printf("Dynamic Scheduling:\n");
    printf("Dot Product = %f\n", dot);
    printf("Time Taken = %f seconds\n", end_dynamic - start_dynamic);

    free(A);
    free(B);

    return 0;
}


ObserVATIONS : 
In static scheduling, loop iterations are divided among threads before execution begins. 
Threads processing computationally heavier iterations take longer,
 while others remain idle at the implicit barrier, causing load imbalance and poor CPU utilization.
  In dynamic scheduling, iterations are assigned to threads at runtime from a shared pool. 
  Threads that finish early receive additional work, 
resulting in better load balancing and reduced overall
 execution time despite minor scheduling overhead.