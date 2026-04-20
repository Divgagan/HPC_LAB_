#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 2000

int main() {
    static double A[N][N], x[N], y[N];

    // Initialization
    for (int i = 0; i < N; i++) {
        x[i] = 1.0;
        y[i] = 0.0;
        for (int j = 0; j < N; j++)
            A[i][j] = 1.0;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int j = 0; j < N; j++) {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }

    double end = omp_get_wtime();

    printf("Row-wise Distribution Time = %f seconds\n", end - start);

    return 0;
}

//  Row-wise multiplication (why it is fast)

// In C, matrix data is stored row by row in memory.

// When we calculate row-wise, the program reads data that is already close together.

// Because of this:

// CPU cache works well

// Data comes quickly

// No extra waiting is needed

// Each thread writes to its own result value, so:

// No two threads fight

// No locks are needed

//  That’s why row-wise runs fast.