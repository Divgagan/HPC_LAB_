/*
 * Lab 11 - Question 3
 * MPI Collective Communication: Reduce & AllReduce (MPI_Reduce / MPI_Allreduce)
 * Compile : mpicc q3_reduce.c -o q3_reduce
 * Run     : mpirun -np 4 ./q3_reduce
 *
 * Problem : Each process contributes a local sum. Use MPI_Reduce to
 *           compute the global sum at root, and MPI_Allreduce so every
 *           process knows the global sum.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ELEMENTS_PER_PROC 5

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Each process creates ELEMENTS_PER_PROC elements and sums them */
    int local_data[ELEMENTS_PER_PROC];
    int local_sum = 0;

    for (int i = 0; i < ELEMENTS_PER_PROC; i++)
    {
        local_data[i] = rank * ELEMENTS_PER_PROC + i + 1;
        local_sum += local_data[i];
    }

    printf("Process %d: local data = [", rank);
    for (int i = 0; i < ELEMENTS_PER_PROC; i++) printf("%d ", local_data[i]);
    printf("], local_sum = %d\n", local_sum);

    /* ---- MPI_Reduce: sum all local_sums -> global_sum at root ---- */
    int global_sum_reduce = 0;
    double t1 = MPI_Wtime();
    MPI_Reduce(&local_sum, &global_sum_reduce, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);
    double t2 = MPI_Wtime();

    if (rank == 0)
        printf("\n[MPI_Reduce]   Global sum at root  = %d  (Time: %f s)\n",
               global_sum_reduce, t2 - t1);

    /* ---- MPI_Allreduce: every process gets the global sum ---- */
    int global_sum_all = 0;
    double t3 = MPI_Wtime();
    MPI_Allreduce(&local_sum, &global_sum_all, 1, MPI_INT,
                  MPI_SUM, MPI_COMM_WORLD);
    double t4 = MPI_Wtime();

    printf("[MPI_Allreduce] Process %d: global_sum = %d  (Time: %f s)\n",
           rank, global_sum_all, t4 - t3);

    /* Also demonstrate MPI_MAX and MPI_MIN */
    int global_max = 0, global_min = 0;
    MPI_Allreduce(&local_sum, &global_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&local_sum, &global_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nMax local_sum across all processes = %d\n", global_max);
        printf("Min local_sum across all processes = %d\n", global_min);
    }

    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * This program demonstrates two all-important MPI reduction
 * collectives: MPI_Reduce and MPI_Allreduce.
 *
 * MPI_REDUCE (All-to-one):
 *   Combines values from all processes using a specified operator
 *   (MPI_SUM, MPI_MAX, MPI_MIN, MPI_PROD, etc.) and deposits the
 *   result ONLY at the root process.
 *   Tree-based implementation: O(log P) communication steps.
 *
 * MPI_ALLREDUCE (All-to-all reduction):
 *   Same as Reduce but the result is distributed back to ALL
 *   processes. Equivalent to Reduce + Broadcast, but optimised
 *   (reduce-scatter + allgather internally).
 *   Every process can use the global result without an extra
 *   broadcast step.
 *
 * OPERATORS DEMONSTRATED:
 *   - MPI_SUM  : global sum of local values
 *   - MPI_MAX  : maximum local_sum across all ranks
 *   - MPI_MIN  : minimum local_sum across all ranks
 *
 * PERFORMANCE COMPARISON:
 *   MPI_Reduce is slightly faster than MPI_Allreduce because
 *   it skips the final broadcast phase. Use MPI_Reduce when only
 *   the root needs the result; use MPI_Allreduce when all
 *   processes need it (e.g., convergence checks in iterative
 *   solvers).
 *
 * USE CASES:
 *   Computing global sums (parallel dot products), finding global
 *   extremes, convergence checking in iterative algorithms (CGM,
 *   GMRES), and distributed statistics.
 * ============================================================
 */
