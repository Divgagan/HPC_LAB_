#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000

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

    int *send_buf = (int *)malloc(N * sizeof(int));
    int *recv_buf = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
        send_buf[i] = rank * 100 + i;

    if (rank == 0)
    {
        printf("Rank 0 trying to send...\n");

        MPI_Send(send_buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Rank 0 finished send\n");

        MPI_Recv(recv_buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else if (rank == 1)
    {
        printf("Rank 1 trying to send...\n");
        MPI_Send(send_buf, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Rank 1 finished send\n");
        MPI_Recv(recv_buf, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    free(send_buf);
    free(recv_buf);

    MPI_Finalize();
}