/* Program 9: MPI_Reduce and MPI_Allreduce with MAX, MIN, SUM, PROD */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, value, max, min, sum, prod;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    value = rank + 1;
    printf("Process %d has value %d\n", rank, value);

    /* MPI_Reduce: result only at root */
    MPI_Reduce(&value, &max,  1, MPI_INT, MPI_MAX,  0, MPI_COMM_WORLD);
    MPI_Reduce(&value, &min,  1, MPI_INT, MPI_MIN,  0, MPI_COMM_WORLD);
    MPI_Reduce(&value, &sum,  1, MPI_INT, MPI_SUM,  0, MPI_COMM_WORLD);
    MPI_Reduce(&value, &prod, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("\n[MPI_Reduce at root] MAX=%d MIN=%d SUM=%d PROD=%d\n\n",
               max, min, sum, prod);

    MPI_Barrier(MPI_COMM_WORLD);

    /* MPI_Allreduce: result at every process */
    MPI_Allreduce(&value, &max,  1, MPI_INT, MPI_MAX,  MPI_COMM_WORLD);
    MPI_Allreduce(&value, &min,  1, MPI_INT, MPI_MIN,  MPI_COMM_WORLD);
    MPI_Allreduce(&value, &sum,  1, MPI_INT, MPI_SUM,  MPI_COMM_WORLD);
    MPI_Allreduce(&value, &prod, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    printf("[MPI_Allreduce] Process %d: MAX=%d MIN=%d SUM=%d PROD=%d\n",
           rank, max, min, sum, prod);

    MPI_Finalize();
    return 0;
}
