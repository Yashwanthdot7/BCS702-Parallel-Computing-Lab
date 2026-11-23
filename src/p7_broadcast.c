/* Program 7: MPI_Bcast */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, data = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = 50;
        printf("Process 0 broadcasting value %d\n", data);
    }

    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received value %d\n", rank, data);

    MPI_Finalize();
    return 0;
}
