#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
    {
        if (rank == 0)
        {
            printf("Error: This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    const int BUFFER_SIZE = 1024 * 1024; // 1 MB
    char *send_buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    char *recv_buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

    if (rank == 0)
        memset(send_buffer, 'A', BUFFER_SIZE);

    // Message Swap
    if (rank == 0)
    {
        printf("0 is sending\n");
        MPI_Send(send_buffer, 1048576, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        printf("0 sent\n");
        // MPI_Recv(recv_buffer, BUFFER_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("2\n");
    }
    else if (rank == 1)
    {
        printf("1 is ready to recieve\n");
        MPI_Recv(recv_buffer, 1048576, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("1 recieved\n");
        // MPI_Send(send_buffer, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        printf("Rank %d received: ", rank);
        for (int i = 0; i < 10; i++)
        {
            printf("%c", recv_buffer[i]);
        }
        printf("1\n");
    }


    free(send_buffer);
    free(recv_buffer);

    MPI_Finalize();
}