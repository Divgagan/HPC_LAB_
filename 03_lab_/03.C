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
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {

            #pragma omp critical
            {
                y[i] += A[i][j] * x[j];
            }
        }
    }

    double end = omp_get_wtime();

    printf("Column-wise Distribution Time = %f seconds\n", end - start);

    return 0;
}



//  Column-wise multiplication (why it is slow)

// Column elements are not close together in memory.

// The program has to jump around in memory to read data.

// This causes:

// More cache misses

// More waiting for memory

// Also, many threads try to update the same result value.

// To stop wrong results, we use critical, which:

// Allows only one thread at a time

// Makes other threads wait

//  This waiting makes the program slow.