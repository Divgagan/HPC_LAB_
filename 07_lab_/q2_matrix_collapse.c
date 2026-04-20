#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500

int main()
{
    static int A[N][N], B[N][N], C[N][N];

    int i, j, k;

    double start, end;

    // Initialize matrices
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    start = omp_get_wtime();

    #pragma omp parallel for collapse(2) private(k)
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            for(k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end = omp_get_wtime();

    printf("Execution Time with collapse(2) = %f seconds\n", end - start);

    printf("Sample Output C[0][0] = %d\n", C[0][0]);

    return 0;
}

// Analysis of the code:
// “In this program, matrix multiplication is implemented using the OpenMP collapse clause.
//  The collapse(2) directive combines two nested loops into a single loop for parallel execution. 
//  This improves load balancing among threads compared to parallelizing only one loop.

// Each thread computes multiple elements of the result matrix, leading to better
//  utilization of CPU resources and improved performance.”



// “Since all elements of matrices A and B are initialized to 1 and the matrix size is 500,
//  each element of the resultant matrix becomes 500 after multiplication.
//   Therefore, the sample output printed is C[0][0] = 500, which verifies the correctness of the program.”