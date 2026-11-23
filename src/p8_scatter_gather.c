/* Program 8: MPI_Scatter and MPI_Gather
 * Root scatters one element to each process, each squares it, root gathers results. */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, recv, *send = NULL, *result = NULL;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        send = malloc(size * sizeof(int));
        result = malloc(size * sizeof(int));
        printf("Root array: ");
        for (int i = 0; i < size; i++) { send[i] = i + 1; printf("%d ", send[i]); }
        printf("\n");
    }

    MPI_Scatter(send, 1, MPI_INT, &recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received %d\n", rank, recv);

    recv = recv * recv;

    MPI_Gather(&recv, 1, MPI_INT, result, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered (squared) array: ");
        for (int i = 0; i < size; i++) printf("%d ", result[i]);
        printf("\n");
        free(send); free(result);
    }

    MPI_Finalize();
    return 0;
}
