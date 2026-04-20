/*
 * Lab 11 - Question 5
 * MPI Collective Communication: Parallel Sum using MPI_Reduce
 * with performance comparison (Sequential vs Parallel)
 * Compile : mpicc q5_parallel_sum.c -o q5_parallel_sum -lm
 * Run     : mpirun -np 4 ./q5_parallel_sum
 *
 * Problem : Compute the sum of 1..N in parallel using scatter + local
 *           summation + reduce, and compare with sequential sum.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000   /* Total number of integers to sum */

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = N / size;      /* Integers per process */

    /* ===================== SEQUENTIAL (only root) ===================== */
    double seq_time = 0.0;
    long long seq_sum = 0;
    if (rank == 0)
    {
        double t1 = MPI_Wtime();
        for (long long i = 1; i <= N; i++)
            seq_sum += i;
        double t2 = MPI_Wtime();
        seq_time = t2 - t1;
        printf("Sequential sum = %lld  (Time: %f s)\n", seq_sum, seq_time);
    }

    /* ===================== PARALLEL ===================== */
    /*
     * Process i handles integers from: (rank * chunk + 1)
     *                              to: (rank+1) * chunk
     * Last process handles any remainder.
     */
    long long start_val = (long long)rank * chunk + 1;
    long long end_val   = (rank == size - 1) ? N : (long long)(rank + 1) * chunk;

    MPI_Barrier(MPI_COMM_WORLD);
    double par_start = MPI_Wtime();

    long long local_sum = 0;
    for (long long i = start_val; i <= end_val; i++)
        local_sum += i;

    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double par_end = MPI_Wtime();

    printf("Process %d: range [%lld, %lld], local_sum = %lld\n",
           rank, start_val, end_val, local_sum);

    if (rank == 0)
    {
        double par_time = par_end - par_start;
        printf("\nParallel sum   = %lld  (Time: %f s)\n", global_sum, par_time);
        printf("Results match  : %s\n", (global_sum == seq_sum) ? "YES" : "NO");
        if (par_time > 0)
            printf("Speedup        : %.2fx\n", seq_time / par_time);
    }

    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * This program benchmarks sequential vs parallel summation of
 * integers from 1 to N (N = 1,000,000) using MPI collectives.
 *
 * APPROACH:
 *   1. Root performs sequential summation as baseline.
 *   2. Each process computes local_sum over its assigned range.
 *   3. MPI_Reduce with MPI_SUM combines all local sums at root.
 *   4. Root prints global_sum and computes speedup.
 *
 * CORRECTNESS:
 *   The expected result is N*(N+1)/2 = 500,000,500,000.
 *   The "Results match: YES" output confirms the parallel
 *   computation is numerically correct.
 *
 * PERFORMANCE:
 *   - With P processes, each process computes N/P additions.
 *   - Computation cost: O(N/P) per process (P× speedup for compute).
 *   - Communication cost: O(log P) for the reduction tree.
 *   - For large N, parallel speedup is close to P (ideal).
 *   - For small N, communication overhead may dominate,
 *     making parallel slower than sequential.
 *
 * LOAD BALANCE:
 *   The last process handles any remainder (N mod P extra elements)
 *   to ensure correctness when N is not evenly divisible by P.
 *
 * USE CASES:
 *   Parallel summation, dot products, integration (numerical
 *   quadrature), and statistical aggregation over large datasets.
 * ============================================================
 */
