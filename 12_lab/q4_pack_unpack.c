/*
 * Lab 12 - Question 4
 * MPI Derived Datatypes: MPI_Pack / MPI_Unpack (manual packing)
 * Compile : mpicc q4_pack_unpack.c -o q4_pack_unpack
 * Run     : mpirun -np 2 ./q4_pack_unpack
 *
 * Problem : Pack heterogeneous data (int, double, char array) into
 *           a single buffer using MPI_Pack, send it, and unpack it
 *           on the receiver side using MPI_Unpack.
 *           This achieves the same goal as MPI_Type_create_struct
 *           but via explicit buffer management.
 */

#include <stdio.h>
#include <string.h>
#include <mpi.h>

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

    /* Data to pack */
    int    id        = 202;
    double value     = 98.6;
    char   label[16] = "SENSOR_02";

    /* Estimate buffer size needed for packing */
    int buf_size;
    int size_int, size_double, size_char;
    MPI_Pack_size(1,  MPI_INT,    MPI_COMM_WORLD, &size_int);
    MPI_Pack_size(1,  MPI_DOUBLE, MPI_COMM_WORLD, &size_double);
    MPI_Pack_size(16, MPI_CHAR,   MPI_COMM_WORLD, &size_char);
    buf_size = size_int + size_double + size_char + MPI_BSEND_OVERHEAD;

    char *buffer = (char *)malloc(buf_size);
    int   position = 0;    /* Tracks current position in buffer */

    if (rank == 0)
    {
        /* ---- PACK: serialise fields into buffer ---- */
        MPI_Pack(&id,    1,  MPI_INT,    buffer, buf_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&value, 1,  MPI_DOUBLE, buffer, buf_size, &position, MPI_COMM_WORLD);
        MPI_Pack(label,  16, MPI_CHAR,   buffer, buf_size, &position, MPI_COMM_WORLD);

        printf("Process 0: Packing id=%d, value=%.1f, label=%s\n",
               id, value, label);
        printf("Process 0: Total packed bytes = %d (buffer_size=%d)\n",
               position, buf_size);

        /* Send the packed buffer as raw bytes */
        MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        MPI_Status status;
        MPI_Recv(buffer, buf_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, &status);

        /* ---- UNPACK: deserialise fields from buffer ---- */
        int    recv_id;
        double recv_value;
        char   recv_label[16];
        position = 0;

        MPI_Unpack(buffer, buf_size, &position, &recv_id,    1,  MPI_INT,    MPI_COMM_WORLD);
        MPI_Unpack(buffer, buf_size, &position, &recv_value, 1,  MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(buffer, buf_size, &position, recv_label,  16, MPI_CHAR,   MPI_COMM_WORLD);

        printf("Process 1: Unpacked id=%d, value=%.1f, label=%s\n",
               recv_id, recv_value, recv_label);
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}

/*
 * ============================================================
 * Analysis of the Code:
 * ============================================================
 * MPI_Pack / MPI_Unpack provide an alternative to derived
 * datatypes for sending heterogeneous data: the programmer
 * manually controls the serialisation into a contiguous byte
 * buffer tagged with the special MPI_PACKED datatype.
 *
 * HOW IT WORKS:
 *   Sender side:
 *     - Allocate a buffer of sufficient size (use MPI_Pack_size
 *       per field and sum them).
 *     - Call MPI_Pack for each field; 'position' advances
 *       automatically after each call.
 *     - Send the buffer with count = position, type = MPI_PACKED.
 *
 *   Receiver side:
 *     - Receive with type MPI_PACKED and buffer large enough.
 *     - Call MPI_Unpack for each field in the same order.
 *     - Fields are deserialised back to their original types.
 *
 * MPI_PACK vs MPI_TYPE_CREATE_STRUCT:
 *   | Aspect         | MPI_Pack/Unpack      | Type_create_struct  |
 *   |----------------|----------------------|---------------------|
 *   | Flexibility    | Fields can vary per  | Fixed layout        |
 *   |                | message dynamically  |                     |
 *   | Performance    | Extra memcpy needed  | Zero-copy possible  |
 *   | Code clarity   | Explicit steps       | Clean, reusable     |
 *
 *   MPI_Pack is useful when the structure of the message changes
 *   dynamically at runtime. MPI_Type_create_struct is preferred
 *   for fixed, frequently reused layouts.
 *
 * USE CASES:
 *   Dynamic record sizes, protocol-style message framing, and
 *   situations where struct layout cannot be known at compile time.
 * ============================================================
 */
