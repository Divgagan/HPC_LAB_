/*
 * Lab 11 - Question 1
 * MPI Collective Communication: Broadcast (MPI_Bcast)
 * Compile : mpicc q1_broadcast.c -o q1_broadcast
 * Run     : mpirun -np 4 ./q1_broadcast
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Root process (rank 0) initializes the data */
    if (rank == 0)
    {
        data = 42;
        printf("Process %d (root): Broadcasting value = %d to all %d processes\n",
               rank, data, size);
    }
    else
    {
        data = -1; /* Other processes start with garbage / sentinel */
    }

    double start = MPI_Wtime();

    /* MPI_Bcast sends 'data' from root (rank 0) to ALL other processes */
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    /* Every process now has the same value */
    printf("Process %d: Received value = %d  (Time: %f s)\n",
           rank, data, end - start);

    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * In this program we demonstrate the MPI_Bcast (Broadcast)
 * collective communication operation.
 *
 * KEY CONCEPT:
 *   MPI_Bcast is a one-to-all collective operation. A single
 *   root process (rank 0) sends one identical message to every
 *   process in the communicator, including itself.
 *
 * HOW IT WORKS:
 *   1. The root process (rank 0) sets data = 42.
 *   2. All other processes initialise data = -1 (sentinel).
 *   3. MPI_Bcast is called by ALL processes simultaneously.
 *      Internally MPI may use a binomial-tree or pipelined
 *      algorithm to distribute the data efficiently.
 *   4. After the call every process holds data = 42.
 *
 * PERFORMANCE:
 *   - A naive sequential broadcast (root sends one-by-one) would
 *     cost O(P) messages.
 *   - MPI's tree-based broadcast costs only O(log P) steps,
 *     making it highly scalable.
 *
 * USE CASES:
 *   Broadcasting a configuration value, an input parameter, or
 *   a converged result to all workers.
 * ============================================================
 */
