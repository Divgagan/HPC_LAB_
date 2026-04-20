#include <mpi.h>
#include <stdio.h>
#include <unistd.h> // for sleep()

#define N 10

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int arr[N];

    if (rank == 0)
    {
        // Initialize array
        for (int i = 0; i < N; i++)
            arr[i] = i;

        printf("Rank 0 sending array to Rank 1\n");

        MPI_Send(arr, N, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(arr, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Rank 0 received modified array:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }
    else if (rank == 1)
    {
        MPI_Recv(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Rank 1 received array:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", arr[i]);
        printf("\n");

        // reverse
        for (int i = 0; i < N / 2; i++)
        {
            int temp = arr[i];
            arr[i] = arr[N - i - 1];
            arr[N - i - 1] = temp;
        }

        printf("Rank 1 sending modified array back\n");

        MPI_Send(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        // Other ranks doing independent work
        for (int i = 0; i < 5; i++)
        {
            printf("Rank %d is doing independent work... step %d\n", rank, i+1);
            sleep(1); // simulate work
        }
    }

    MPI_Finalize();
}