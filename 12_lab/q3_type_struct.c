/*
 * Lab 12 - Question 3
 * MPI Derived Datatypes: MPI_Type_struct (for C structures)
 * Compile : mpicc q3_type_struct.c -o q3_type_struct
 * Run     : mpirun -np 2 ./q3_type_struct
 *
 * Problem : Define a C struct { int id; double temperature; char city[20]; }
 *           Create an MPI_Type_struct derived datatype and send one
 *           struct from process 0 to process 1.
 */

#include <stdio.h>
#include <string.h>
#include <mpi.h>

/* ----- User-defined structure ----- */
typedef struct
{
    int    id;
    double temperature;
    char   city[20];
} WeatherRecord;

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

    /* ----- Step 1: Describe the struct to MPI ----- */

    /* Number of fields in the struct */
    const int num_fields = 3;

    /* Count of each field */
    int          blocklens[3]  = {1, 1, 20};

    /* MPI types of each field */
    MPI_Datatype types[3]      = {MPI_INT, MPI_DOUBLE, MPI_CHAR};

    /* Byte offsets of each field within the struct */
    MPI_Aint     displacements[3];
    WeatherRecord dummy;                    /* Used only for offset computation */
    MPI_Aint base_addr;
    MPI_Get_address(&dummy,             &base_addr);
    MPI_Get_address(&dummy.id,          &displacements[0]);
    MPI_Get_address(&dummy.temperature, &displacements[1]);
    MPI_Get_address(&dummy.city,        &displacements[2]);

    /* Convert to relative offsets */
    displacements[0] -= base_addr;
    displacements[1] -= base_addr;
    displacements[2] -= base_addr;

    /* Create the derived struct datatype */
    MPI_Datatype weather_type;
    MPI_Type_create_struct(num_fields, blocklens, displacements, types, &weather_type);
    MPI_Type_commit(&weather_type);

    /* ----- Step 2: Use the derived type ----- */
    WeatherRecord record;

    if (rank == 0)
    {
        record.id          = 101;
        record.temperature = 36.5;
        strncpy(record.city, "Bangalore", sizeof(record.city) - 1);
        record.city[sizeof(record.city) - 1] = '\0';

        printf("Process 0 sending: ID=%d, Temp=%.1f, City=%s\n",
               record.id, record.temperature, record.city);

        MPI_Send(&record, 1, weather_type, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        MPI_Recv(&record, 1, weather_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process 1 received: ID=%d, Temp=%.1f, City=%s\n",
               record.id, record.temperature, record.city);
    }

    MPI_Type_free(&weather_type);
    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * MPI_Type_create_struct is the most powerful MPI derived
 * datatype constructor. It allows you to describe arbitrary C
 * structures with mixed types and compiler-inserted padding so
 * MPI can send them safely across processes (and even machines).
 *
 * THE PROBLEM WITH SENDING STRUCTS DIRECTLY:
 *   Sending &record with MPI_BYTE would work within a single
 *   machine, but breaks in heterogeneous environments because:
 *   1. Compiler padding may differ between compilers/platforms.
 *   2. Endianness may differ between architectures.
 *   MPI_Type_create_struct is the portable, correct solution.
 *
 * KEY STEPS:
 *   1. blocklens[]   — how many elements of each type per field
 *   2. displacements[] — byte offset of each field (via MPI_Get_address)
 *   3. types[]        — MPI type of each field
 *   4. MPI_Type_create_struct — assembles these into a new type
 *   5. MPI_Type_commit         — registers with the MPI runtime
 *   6. MPI_Type_free           — cleanup after use
 *
 * MPI_Get_address vs offsetof:
 *   MPI_Get_address is preferred over manual offsetof() because
 *   it correctly handles all MPI address widths (MPI_Aint).
 *
 * PERFORMANCE:
 *   Struct types have negligible overhead once committed. MPI
 *   caches the type layout internally. For large arrays of
 *   structs, use MPI_Type_create_struct inside an MPI_Type_contiguous
 *   or MPI_Type_vector to send entire struct arrays efficiently.
 *
 * USE CASES:
 *   Scientific simulations (particle structs, record transfer),
 *   distributed databases, any application passing heterogeneous
 *   C structures between MPI processes.
 * ============================================================
 */
