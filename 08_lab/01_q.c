#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int value = 500;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(argc > 1)
        value = atoi(argv[1]);

    if(rank == 0) 
    {
        printf("Rank 0 sending %d to Rank 1\n", value);

        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    else if(rank == size-1)
    {
        MPI_Recv(&value, 1, MPI_INT, rank-1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Final Rank %d received %d\n", rank, value);
    }

    else 
    {
        MPI_Recv(&value, 1, MPI_INT, rank-1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Rank %d received %d from Rank %d\n",
               rank, value, rank-1);

        MPI_Send(&value, 1, MPI_INT, rank+1, 0,
                 MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}