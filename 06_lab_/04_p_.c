#include <stdio.h>
#include <omp.h>

#define N 4

int main() {
    int A[N][N], x[N], y[N] = {0};

    // Initialization (small values)
    for (int i = 0; i < N; i++) {
        x[i] = i + 1;
        for (int j = 0; j < N; j++) {
            A[i][j] = j + 1;
        }
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {
                #pragma omp task depend(out: y[i])
                {
                    for (int j = 0; j < N; j++) {
                        y[i] += A[i][j] * x[j];
                    }
                }
            }
        }
    }

    printf("Result vector y:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", y[i]);
    }
    printf("\n");

    return 0;
}

// Analysis:01 
// This program performs a matrix-vector multiplication using OpenMP tasks with dependencies.
// Each task computes one element of the result vector y, and the depend clause ensures that
// tasks are executed in the correct order. The single directive ensures that only one thread
// creates the tasks, while the parallel region allows multiple threads to execute them concurrently.  

// Analysis _02 : 
// The matrix-vector multiplication is decomposed into tasks, where each task computes one row of the result. 
// The depend clause ensures proper data flow and avoids race conditions. This allows efficient parallel execution 
// while maintaining correctness.