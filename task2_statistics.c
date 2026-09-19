#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    enum { N = 12 };

    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* N harus habis dibagi jumlah proses */
    if (size > N || N % size != 0)
    {
        if (rank == 0)
            fprintf(stderr,
                    "Gunakan jumlah proses yang membagi 12, "
                    "misalnya 1, 2, 3, 4, 6, atau 12.\n");

        MPI_Finalize();
        return 1;
    }

    /* Hanya Rank 0 yang membuat data */
    int data[N] = {0};

    if (rank == 0)
    {
        for (int i = 0; i < N; ++i)
            data[i] = i + 1;
    }

    int count = N / size;

    int local[N] = {0};

    /* Membagi data ke semua rank */
    MPI_Scatter(
        data,
        count,
        MPI_INT,
        local,
        count,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    /* Hitung jumlah lokal */
    int local_sum = 0;

    /* Cari nilai maksimum lokal */
    int local_max = local[0];

    for (int i = 0; i < count; ++i)
    {
        local_sum += local[i];

        if (local[i] > local_max)
            local_max = local[i];
    }

    /* Gabungkan semua local_sum menjadi global sum */
    int global_sum = 0;

    MPI_Reduce(
        &local_sum,
        &global_sum,
        1,
        MPI_INT,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    /* Gabungkan semua local_max menjadi global maximum */
    int global_max = 0;

    MPI_Reduce(
        &local_max,
        &global_max,
        1,
        MPI_INT,
        MPI_MAX,
        0,
        MPI_COMM_WORLD
    );

    /* Hanya Rank 0 yang mencetak hasil global */
    if (rank == 0)
    {
        printf("Global sum: %d\n", global_sum);
        printf("Global maximum: %d\n", global_max);
    }

    MPI_Finalize();

    return 0;
}
