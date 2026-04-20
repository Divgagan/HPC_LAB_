/*
 * Lab 12 - Question 5
 * MPI Derived Datatypes: Parallel Matrix Distribution using
 * MPI_Type_vector (scatter rows) + MPI_Reduce (parallel row sum)
 *
 * Compile : mpicc q5_matrix_row_sum.c -o q5_matrix_row_sum
 * Run     : mpirun -np 4 ./q5_matrix_row_sum
 *
 * Problem : A 4×4 matrix is distributed row-wise to 4 processes.
 *           Each process computes the sum of its row.
 *           Root collects all row sums using MPI_Gather.
 *           Demonstrates derived datatypes combined with collectives.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 4
#define COLS 4

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != ROWS)
    {
        if (rank == 0)
            printf("Run with exactly %d processes (one per row).\n", ROWS);
        MPI_Finalize();
        return 1;
    }

    /* ---- Step 1: Root creates and prints the matrix ---- */
    int matrix[ROWS][COLS];
    int row_buf[COLS];    /* Each process receives one row */

    if (rank == 0)
    {
        printf("Root matrix:\n");
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                matrix[i][j] = i * COLS + j + 1;
                printf("%3d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    /* ---- Step 2: Create a derived type for one row (contiguous) ---- */
    MPI_Datatype row_type;
    MPI_Type_contiguous(COLS, MPI_INT, &row_type);
    MPI_Type_commit(&row_type);

    /* ---- Step 3: Scatter rows → each process gets one row ---- */
    /*
     * MPI_Scatter with row_type:
     *   Sends 1 row_type element (= COLS ints) to each process.
     *   Process i receives matrix[i][0..COLS-1].
     */
    MPI_Scatter(matrix,   1, row_type,
                row_buf,  1, row_type,
                0, MPI_COMM_WORLD);

    printf("Process %d row: ", rank);
    for (int j = 0; j < COLS; j++) printf("%d ", row_buf[j]);
    printf("\n");

    /* ---- Step 4: Each process computes local row sum ---- */
    int local_sum = 0;
    for (int j = 0; j < COLS; j++)
        local_sum += row_buf[j];

    printf("Process %d: Row sum = %d\n", rank, local_sum);

    /* ---- Step 5: Gather all row sums at root ---- */
    int all_sums[ROWS];
    MPI_Gather(&local_sum, 1, MPI_INT,
               all_sums,   1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int grand_total = 0;
        printf("\nRow sums gathered at root:\n");
        for (int i = 0; i < ROWS; i++)
        {
            printf("  Row %d sum = %d\n", i, all_sums[i]);
            grand_total += all_sums[i];
        }
        printf("Grand total of all elements = %d\n", grand_total);
        /* Verify: sum of 1..N*N = N*N*(N*N+1)/2 = 4*4*(16+1)/2 = 136 */
        printf("Expected total (1..%d) = %d\n", ROWS*COLS, ROWS*COLS*(ROWS*COLS+1)/2);
    }

    MPI_Type_free(&row_type);
    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * This program combines MPI derived datatypes with collective
 * operations to implement parallel row-wise matrix processing.
 *
 * DESIGN OVERVIEW:
 *   1. Root initialises a ROWS×COLS matrix.
 *   2. A derived 'row_type' (MPI_Type_contiguous of COLS ints)
 *      abstracts one matrix row.
 *   3. MPI_Scatter distributes exactly one row per process using
 *      row_type, making the Scatter call clean and readable.
 *   4. Each process independently computes its row sum in O(COLS).
 *   5. MPI_Gather collects all row sums to the root.
 *
 * WHY USE A DERIVED TYPE FOR SCATTER?
 *   We could achieve the same with MPI_Scatter(matrix, COLS, MPI_INT,
 *   ...) directly. However using a derived 'row_type' demonstrates
 *   how derived types make code more abstract, self-documenting,
 *   and easier to scale to 2D blocks or non-contiguous layouts.
 *
 * CORRECTNESS:
 *   The grand total (136 for a 4×4 matrix filled 1..16) can be
 *   verified with the formula N*(N+1)/2 where N = ROWS*COLS.
 *
 * SCALABILITY:
 *   For larger matrices (e.g. 1000×1000 with 100 processes),
 *   this pattern scales to 100 rows per process, and the
 *   communication cost (Scatter + Gather) remains O(log P).
 *
 * USE CASES:
 *   Parallel matrix row operations, sparse matrix-vector products,
 *   distributed image processing (row of pixels per process),
 *   and block-row distributed linear solvers.
 * ============================================================
 */
