/*
 * Lab 11 - Question 4
 * MPI Collective Communication: AlltoAll (MPI_Alltoall)
 * Compile : mpicc q4_alltoall.c -o q4_alltoall
 * Run     : mpirun -np 4 ./q4_alltoall
 *
 * Problem : Each process sends a DIFFERENT value to each other process.
 *           Demonstrates the "all-to-all personalized communication" pattern
 *           (matrix transpose analogy).
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /*
     * send_buf[j] = value this process wants to send to process j
     * recv_buf[j] = value received from process j
     */
    int *send_buf = (int *)malloc(size * sizeof(int));
    int *recv_buf = (int *)malloc(size * sizeof(int));

    /* Initialise: process i sends value (rank*10 + j) to process j */
    printf("Process %d sends: ", rank);
    for (int j = 0; j < size; j++)
    {
        send_buf[j] = rank * 10 + j;
        printf("[to P%d]=%d ", j, send_buf[j]);
    }
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD); /* Synchronise output for clarity */

    double start = MPI_Wtime();

    /* MPI_Alltoall: every process sends 1 int to every other process */
    MPI_Alltoall(send_buf, 1, MPI_INT,
                 recv_buf, 1, MPI_INT,
                 MPI_COMM_WORLD);

    double end = MPI_Wtime();

    printf("Process %d received: ", rank);
    for (int j = 0; j < size; j++)
        printf("[from P%d]=%d ", j, recv_buf[j]);
    printf("  (Time: %f s)\n", end - start);

    free(send_buf);
    free(recv_buf);
    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * MPI_Alltoall is the most general collective: every process
 * sends a (potentially different) message to every other process.
 * It is the collective equivalent of a full matrix transpose.
 *
 * HOW IT WORKS:
 *   If we have P processes, each holding P values, after Alltoall:
 *     recv_buf[j] at process i  ==  send_buf[i] at process j
 *   i.e., element [i][j] and [j][i] are exchanged, like transposing
 *   a P×P logical matrix.
 *
 * ALGORITHM:
 *   MPI implementations use various algorithms depending on P and
 *   message size:
 *   - Bruck's algorithm: O(P log P) messages, good for small msgs
 *   - Pairwise exchange:  O(P) steps, good for large messages
 *
 * PERFORMANCE:
 *   MPI_Alltoall is the most communication-intensive collective,
 *   involving P*(P-1) point-to-point transfers. On a network with
 *   bandwidth B, the time is approximately (P-1) * msg_size / B.
 *   This makes it the bottleneck in many distributed algorithms.
 *
 * USE CASES:
 *   - FFT communication phase (distributing frequency bins)
 *   - Parallel matrix transpose
 *   - Bucket sort redistribution phase
 *   - All-pairs computation patterns
 * ============================================================
 */
