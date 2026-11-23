/* Program 5: MPI_Send and MPI_Recv (run with 2+ processes) */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, data;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) printf("Run with at least 2 processes\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        data = 100;
        for (int dest = 1; dest < size; dest++) {
            MPI_Send(&data, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            printf("Process 0 sent %d to process %d\n", data, dest);
        }
    } else {
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d from process 0\n", rank, data);
    }

    MPI_Finalize();
    return 0;
}
