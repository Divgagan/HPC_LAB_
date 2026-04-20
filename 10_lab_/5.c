#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *send_buf = (int *)malloc(N * sizeof(int));
    int *recv_buf = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
        send_buf[i] = rank * 100 + i;

    MPI_Barrier(MPI_COMM_WORLD);
    double start_block = MPI_Wtime();

    if (rank == 0)
    {
        MPI_Send(send_buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(recv_buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
        MPI_Recv(recv_buf, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(send_buf, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    double end_block = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    double start_nonblock = MPI_Wtime();

    MPI_Request reqs[2];

    MPI_Isend(send_buf, N, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Irecv(recv_buf, N, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD, &reqs[1]);

    MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);

    double end_nonblock = MPI_Wtime();

    if (rank == 0)
    {
        printf("Blocking time   : %f seconds\n", end_block - start_block);
        printf("Non-blocking time: %f seconds\n", end_nonblock - start_nonblock);
    }

    free(send_buf);
    free(recv_buf);

    MPI_Finalize();
    return 0;
}