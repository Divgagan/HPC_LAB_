#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int value = 100;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 16)
    {
        if(rank == 0)
            printf("Error: Must run with exactly 16 processes.\n");

        MPI_Finalize();
        return 0;
    }

    int mask = 1;

    while(mask < size)
    {
        if(rank < mask)
        {
            MPI_Send(&value, 1, MPI_INT,
                     rank + mask, 0, MPI_COMM_WORLD);
        }

        else if(rank < 2*mask)
        {
            MPI_Recv(&value, 1, MPI_INT,
                     rank - mask, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        mask *= 2;
    }

    printf("Rank %d has value %d\n", rank, value);

    MPI_Finalize();

    return 0;
}