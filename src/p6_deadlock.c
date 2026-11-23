/* Program 6: MPI deadlock and how to avoid it by changing the call order.
 * Run with 2 processes:
 *   mpiexec -n 2 p6_deadlock.exe 1   -> PART 1: deadlock (hangs, press Ctrl+C)
 *   mpiexec -n 2 p6_deadlock.exe 2   -> PART 2: deadlock avoided
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, num = 123;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) printf("Run with at least 2 processes\n");
        MPI_Finalize();
        return 0;
    }

    int part = (argc > 1) ? atoi(argv[1]) : 2;

    if (part == 1) {
        /* PART 1: both processes RECEIVE first -> each waits for the other forever */
        if (rank == 0) {
            printf("Process 0 waiting to receive from Process 1...\n");
            fflush(stdout);
            MPI_Recv(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        } else if (rank == 1) {
            printf("Process 1 waiting to receive from Process 0...\n");
            fflush(stdout);
            MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    } else {
        /* PART 2: change the order - process 0 sends first, process 1 receives first */
        if (rank == 0) {
            MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process 0 received back number: %d\n", num);
        } else if (rank == 1) {
            MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            printf("Process 1 received and sent back number: %d\n", num);
        }
    }

    MPI_Finalize();
    return 0;
}
