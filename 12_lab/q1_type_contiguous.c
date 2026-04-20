/*
 * Lab 12 - Question 1
 * MPI Derived Datatypes: MPI_Type_contiguous
 * Compile : mpicc q1_type_contiguous.c -o q1_type_contiguous
 * Run     : mpirun -np 2 ./q1_type_contiguous
 *
 * Problem : Send a block of 5 integers from process 0 to process 1
 *           using a custom contiguous derived datatype.
 */

#include <stdio.h>
#include <mpi.h>

#define COUNT 5

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

    /* Step 1: Define a contiguous derived datatype = COUNT contiguous ints */
    MPI_Datatype block_type;
    MPI_Type_contiguous(COUNT, MPI_INT, &block_type);
    MPI_Type_commit(&block_type);   /* Must commit before use */

    int data[COUNT];

    if (rank == 0)
    {
        /* Initialise data */
        for (int i = 0; i < COUNT; i++) data[i] = (i + 1) * 10;

        printf("Process 0 sending: ");
        for (int i = 0; i < COUNT; i++) printf("%d ", data[i]);
        printf("\n");

        /* Send 1 element of type block_type (= COUNT ints) */
        MPI_Send(data, 1, block_type, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        MPI_Recv(data, 1, block_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process 1 received: ");
        for (int i = 0; i < COUNT; i++) printf("%d ", data[i]);
        printf("\n");
    }

    /* Step 2: Free the derived datatype when done */
    MPI_Type_free(&block_type);

    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * MPI_Type_contiguous is the simplest MPI derived datatype.
 * It creates a new type by replicating an existing type a fixed
 * number of times in contiguous memory.
 *
 * HOW IT WORKS:
 *   MPI_Type_contiguous(5, MPI_INT, &block_type) creates a type
 *   that spans 5 * sizeof(int) = 20 contiguous bytes. Sending
 *   1 element of block_type is equivalent to sending 5 MPI_INTs.
 *
 * WORKFLOW:
 *   1. MPI_Type_contiguous  — define the new type
 *   2. MPI_Type_commit       — register it with MPI runtime
 *   3. Use in Send/Recv      — as any built-in type
 *   4. MPI_Type_free         — release resources when done
 *
 * WHY DERIVED DATATYPES?
 *   They let you describe non-standard memory layouts (structs,
 *   column-major slices, strided arrays) and send them in a
 *   single MPI call, avoiding manual packing/unpacking.
 *
 * PERFORMANCE:
 *   For contiguous data this is equivalent to sending COUNT
 *   MPI_INTs directly. The real benefit emerges with strided or
 *   struct-based types that would otherwise require user-level
 *   pack/unpack loops.
 * ============================================================
 */
