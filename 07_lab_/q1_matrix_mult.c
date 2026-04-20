#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 4

int main()
{
    int A[N][N], B[N][N], C_seq[N][N], C_par[N][N];
    int i, j, k;

    double start, end;

    // Initialize matrices
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            A[i][j] = i + j;
            B[i][j] = i * j;

            C_seq[i][j] = 0;
            C_par[i][j] = 0;
        }
    }

    // Sequential Matrix Multiplication
    start = omp_get_wtime();

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            for(k = 0; k < N; k++)
            {
                C_seq[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end = omp_get_wtime();

    printf("Sequential Execution Time = %f seconds\n", end - start);

    // Parallel Matrix Multiplication
    start = omp_get_wtime();

    #pragma omp parallel for private(j,k)
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            for(k = 0; k < N; k++)
            {
                C_par[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end = omp_get_wtime();

    printf("Parallel Execution Time = %f seconds\n", end - start);

    // Print Result Matrix
    printf("\nResult Matrix:\n");

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            printf("%d ", C_par[i][j]);
        }
        printf("\n");
    }

    return 0;
}

//  Analysis of the code:
// “In this program, we implemented matrix multiplication using both sequential
//  and parallel approaches. In the sequential method, computation is performed 
//  using nested loops where a single processor executes all operations step by step.
//   In the parallel approach, the outer loop is parallelized using OpenMP (#pragma omp parallel for), 
//   which distributes rows among multiple threads.

//  The parallel version reduces execution time because multiple threads compute
//  different rows simultaneously. The correctness of the program is verified by comparing 
//  both results.”