#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
    {
        if (rank == 0)
            printf("Run with exactly 2 processes\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
    {
        double send_val = 12345.6789;  // 8 bytes
        printf("Rank 0 sending double: %lf\n", send_val);

        MPI_Send(&send_val, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        float recv_val[2];  
        MPI_Recv(recv_val, 2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Rank 1 received values (corrupted): %f %f\n", recv_val[0], recv_val[1]);
    }

    MPI_Finalize();
}