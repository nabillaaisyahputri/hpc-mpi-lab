#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int data[5];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
    {
        if (rank == 0)
            printf("Program harus dijalankan dengan 2 proses.\n");

        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        /* Data awal */
        for (int i = 0; i < 5; i++)
        {
            data[i] = i + 1;
        }

        printf("Rank 0 mengirim: ");

        for (int i = 0; i < 5; i++)
        {
            printf("%d ", data[i]);
        }

        printf("\n");

        /* Kirim array ke Rank 1 */
        MPI_Send(data, 5, MPI_INT, 1, 0, MPI_COMM_WORLD);

        /* Terima hasil dari Rank 1 */
        MPI_Recv(data, 5, MPI_INT, 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        printf("Rank 0 menerima: ");

        for (int i = 0; i < 5; i++)
        {
            printf("%d ", data[i]);
        }

        printf("\n");
    }
    else if (rank == 1)
    {
        /* Terima array dari Rank 0 */
        MPI_Recv(data, 5, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        /* Kuadratkan setiap elemen */
        for (int i = 0; i < 5; i++)
        {
            data[i] = data[i] * data[i];
        }

        /* Kirim kembali ke Rank 0 */
        MPI_Send(data, 5, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
