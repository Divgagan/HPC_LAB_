/*
 * Lab 11 - Question 2
 * MPI Collective Communication: Scatter & Gather (MPI_Scatter / MPI_Gather)
 * Compile : mpicc q2_scatter_gather.c -o q2_scatter_gather
 * Run     : mpirun -np 4 ./q2_scatter_gather
 *
 * Problem : Root scatters an array among all processes. Each process
 *           doubles its chunk. Root then gathers results back.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 8   /* Total elements; must be divisible by number of processes */

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0)
    {
        if (rank == 0)
            printf("Error: N (%d) must be divisible by number of processes (%d)\n", N, size);
        MPI_Finalize();
        return 1;
    }

    int chunk = N / size;           /* Elements each process will receive */

    int *global_arr   = NULL;       /* Only root allocates full array     */
    int *result_arr   = NULL;
    int *local_chunk  = (int *)malloc(chunk * sizeof(int));
    int *local_result = (int *)malloc(chunk * sizeof(int));

    /* Root initialises the global array */
    if (rank == 0)
    {
        global_arr  = (int *)malloc(N * sizeof(int));
        result_arr  = (int *)malloc(N * sizeof(int));
        printf("Root: Original array: ");
        for (int i = 0; i < N; i++)
        {
            global_arr[i] = i + 1;       /* {1,2,3,...,N} */
            printf("%d ", global_arr[i]);
        }
        printf("\n");
    }

    double start = MPI_Wtime();

    /* ---- SCATTER: distribute chunks from root to all processes ---- */
    MPI_Scatter(global_arr, chunk, MPI_INT,
                local_chunk, chunk, MPI_INT,
                0, MPI_COMM_WORLD);

    /* Each process doubles its own chunk */
    for (int i = 0; i < chunk; i++)
        local_result[i] = local_chunk[i] * 2;

    printf("Process %d: Received chunk [", rank);
    for (int i = 0; i < chunk; i++) printf("%d ", local_chunk[i]);
    printf("] -> Doubled [");
    for (int i = 0; i < chunk; i++) printf("%d ", local_result[i]);
    printf("]\n");

    /* ---- GATHER: collect results from all processes to root ---- */
    MPI_Gather(local_result, chunk, MPI_INT,
               result_arr,   chunk, MPI_INT,
               0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0)
    {
        printf("Root: Gathered result: ");
        for (int i = 0; i < N; i++)
            printf("%d ", result_arr[i]);
        printf("\n");
        printf("Total time (Scatter + Compute + Gather): %f s\n", end - start);
        free(global_arr);
        free(result_arr);
    }

    free(local_chunk);
    free(local_result);
    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * This program demonstrates two complementary MPI collectives:
 * MPI_Scatter and MPI_Gather, forming the classic parallel
 * "distribute-compute-collect" pattern.
 *
 * MPI_SCATTER (One-to-all, different data):
 *   The root process divides the global array into equal chunks
 *   and sends a DIFFERENT chunk to each process (including itself).
 *   Unlike Broadcast, each process receives a unique portion.
 *   Internally MPI_Scatter may use a binomial-tree or direct
 *   send algorithm.
 *
 * LOCAL COMPUTATION:
 *   Each process independently doubles its received chunk.
 *   This is where true parallelism lies - all P processes work
 *   simultaneously on separate data segments.
 *
 * MPI_GATHER (All-to-one, inverse of Scatter):
 *   Each process sends its local result back to the root.
 *   The root reassembles results in rank order, reconstructing
 *   the complete output array.
 *
 * PERFORMANCE:
 *   Both Scatter and Gather cost O(log P) steps in a tree-based
 *   implementation. The parallel computation reduces wall-clock
 *   time proportionally to the number of processes.
 *
 * USE CASES:
 *   Parallel array processing, parallel sort (local sort phase),
 *   distributed numerical integration, and parallel prefix sums.
 * ============================================================
 */
