/*
 * Lab 12 - Question 2
 * MPI Derived Datatypes: MPI_Type_vector (strided/column-major)
 * Compile : mpicc q2_type_vector.c -o q2_type_vector
 * Run     : mpirun -np 2 ./q2_type_vector
 *
 * Problem : Send a COLUMN of a 2D row-major matrix from process 0
 *           to process 1 using MPI_Type_vector.
 *
 *           Matrix (4×4, row-major in memory):
 *           [ 1  2  3  4 ]
 *           [ 5  6  7  8 ]
 *           [ 9 10 11 12 ]
 *           [13 14 15 16 ]
 *
 *           Goal: send column 1 → {2, 6, 10, 14}
 */

#include <stdio.h>
#include <mpi.h>

#define ROWS 4
#define COLS 4
#define COL_TO_SEND 1   /* 0-indexed column index */

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        if (rank == 0) printf("Run with at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    /*
     * MPI_Type_vector parameters:
     *   count   = ROWS  (number of blocks = number of rows)
     *   bloclen = 1     (1 element per block = one element per column)
     *   stride  = COLS  (gap between starts of successive blocks = row length)
     */
    MPI_Datatype col_type;
    MPI_Type_vector(ROWS, 1, COLS, MPI_INT, &col_type);
    MPI_Type_commit(&col_type);

    int matrix[ROWS][COLS];
    int col_recv[ROWS];

    if (rank == 0)
    {
        /* Fill matrix: matrix[i][j] = i*COLS + j + 1 */
        int val = 1;
        printf("Process 0: Matrix:\n");
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                matrix[i][j] = val++;
                printf("%3d ", matrix[i][j]);
            }
            printf("\n");
        }

        /* Start sending from element matrix[0][COL_TO_SEND] */
        printf("\nProcess 0: Sending column %d → ", COL_TO_SEND);
        for (int i = 0; i < ROWS; i++) printf("%d ", matrix[i][COL_TO_SEND]);
        printf("\n");

        MPI_Send(&matrix[0][COL_TO_SEND], 1, col_type, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        /* Receive into a contiguous array */
        MPI_Recv(col_recv, ROWS, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process 1: Received column %d: ", COL_TO_SEND);
        for (int i = 0; i < ROWS; i++) printf("%d ", col_recv[i]);
        printf("\n");
    }

    MPI_Type_free(&col_type);
    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * MPI_Type_vector solves a critical problem in parallel matrix
 * computations: how to send non-contiguous data (like a column
 * of a row-major matrix) without manual copying.
 *
 * MEMORY LAYOUT OF ROW-MAJOR MATRIX:
 *   In C, a 4×4 matrix is stored as:
 *   [1,2,3,4, | 5,6,7,8, | 9,10,11,12, | 13,14,15,16]
 *    col 0123   col 0123    col 0123      col 0123
 *
 *   Column 1 elements (2, 6, 10, 14) are STRIDE apart in memory,
 *   where stride = COLS = 4 integers.
 *
 * MPI_TYPE_VECTOR PARAMETERS:
 *   count   = ROWS  (4 blocks, one per row)
 *   bloclen = 1     (take 1 integer from each row)
 *   stride  = COLS  (jump COLS ints between blocks)
 *
 * WHY THIS MATTERS:
 *   Without MPI_Type_vector, you would need to:
 *     - Copy column into a temporary buffer (memcpy loop)
 *     - Send the buffer
 *     - Receive and scatter back
 *   MPI_Type_vector eliminates the copy, reducing memory usage
 *   and improving cache behaviour.
 *
 * RECEIVE SIDE:
 *   The receiver uses a plain MPI_INT array (contiguous) since
 *   it only needs the values, not the strided layout.
 *
 * USE CASES:
 *   Parallel matrix operations (LU, QR factorization), sending
 *   rows/columns/subblocks of distributed matrices, ghost-cell
 *   exchange in finite-difference stencils.
 * ============================================================
 */
