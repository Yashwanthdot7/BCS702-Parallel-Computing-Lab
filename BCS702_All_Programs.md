# BCS702 Parallel Computing Lab - Programs 1 to 9

Run the commands in **Command Prompt (cmd)**, from the folder where the .c file is saved.

- OpenMP (1-4): needs gcc (MinGW / CodeBlocks MinGW).
- MPI (5-9): needs gcc + MS-MPI runtime (msmpisetup.exe) + MS-MPI SDK (msmpisdk.msi).
- If your gcc is 32-bit, use %MSMPI_LIB32% instead of %MSMPI_LIB64%.

---

## Program 1: Merge sort - sequential vs parallel (OpenMP sections)
File: `p1.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int a[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = malloc(n1 * sizeof(int)), *R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = a[l + i];
    for (int j = 0; j < n2; j++) R[j] = a[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) a[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) a[k++] = L[i++];
    while (j < n2) a[k++] = R[j++];
    free(L); free(R);
}

void seqMergeSort(int a[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        seqMergeSort(a, l, m);
        seqMergeSort(a, m + 1, r);
        merge(a, l, m, r);
    }
}

void parMergeSort(int a[], int l, int r, int depth) {
    if (l < r) {
        int m = (l + r) / 2;
        if (depth > 0) {
            #pragma omp parallel sections
            {
                #pragma omp section
                parMergeSort(a, l, m, depth - 1);
                #pragma omp section
                parMergeSort(a, m + 1, r, depth - 1);
            }
        } else {
            seqMergeSort(a, l, m);
            seqMergeSort(a, m + 1, r);
        }
        merge(a, l, m, r);
    }
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int *a = malloc(n * sizeof(int)), *b = malloc(n * sizeof(int));
    srand(42);
    for (int i = 0; i < n; i++) a[i] = b[i] = rand() % 100000;

    omp_set_nested(1);

    double t1 = omp_get_wtime();
    seqMergeSort(a, 0, n - 1);
    double seqTime = omp_get_wtime() - t1;

    t1 = omp_get_wtime();
    parMergeSort(b, 0, n - 1, 3);
    double parTime = omp_get_wtime() - t1;

    if (n <= 20) {
        printf("Sorted array: ");
        for (int i = 0; i < n; i++) printf("%d ", b[i]);
        printf("\n");
    }
    printf("Sequential time : %f s\n", seqTime);
    printf("Parallel time   : %f s\n", parTime);
    printf("Difference      : %f s\n", seqTime - parTime);

    free(a); free(b);
    return 0;
}
```

**Compile:**
```
gcc -fopenmp p1.c -o p1.exe
```

**Run:**
```
.\p1.exe
```
Sample input: `1000000`

---

## Program 2: Static schedule, chunk size 2 (OpenMP)
File: `p2.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter number of iterations: ");
    scanf("%d", &n);

    int *owner = malloc(n * sizeof(int));

    omp_set_schedule(omp_sched_static, 2);

    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < n; i++) {
        owner[i] = omp_get_thread_num();
    }

    for (int i = 0; i < n; ) {
        int start = i;
        while (i + 1 < n && owner[i + 1] == owner[start] && (i + 1 - start) < 2) i++;
        printf("Thread %d : Iterations %d -- %d\n", owner[start], start, i);
        i++;
    }

    free(owner);
    return 0;
}
```

**Compile:**
```
gcc -fopenmp p2.c -o p2.exe
```

**Run:**
```
.\p2.exe
```
Sample input: `8`

---

## Program 3: Fibonacci using tasks (OpenMP)
File: `p3.c`

```c
#include <stdio.h>
#include <omp.h>

long fib(int n) {
    long x, y;
    if (n < 2) return n;
    #pragma omp task shared(x) if(n > 20)
    x = fib(n - 1);
    #pragma omp task shared(y) if(n > 20)
    y = fib(n - 2);
    #pragma omp taskwait
    return x + y;
}

int main() {
    int n;
    printf("Enter number of Fibonacci terms: ");
    scanf("%d", &n);

    long result[100];
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < n; i++) {
                #pragma omp task firstprivate(i)
                result[i] = fib(i);
            }
            #pragma omp taskwait
        }
    }

    printf("Fibonacci series: ");
    for (int i = 0; i < n; i++) printf("%ld ", result[i]);
    printf("\n");
    return 0;
}
```

**Compile:**
```
gcc -fopenmp p3.c -o p3.exe
```

**Run:**
```
.\p3.exe
```
Sample input: `10`

---

## Program 4: Prime numbers 1 to n - serial vs parallel (OpenMP)
File: `p4.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int isPrime(int x) {
    if (x < 2) return 0;
    for (int i = 2; i * i <= x; i++)
        if (x % i == 0) return 0;
    return 1;
}

int main() {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    int *flag = calloc(n + 1, sizeof(int));
    int countS = 0, countP = 0;

    double t = omp_get_wtime();
    for (int i = 1; i <= n; i++)
        if (isPrime(i)) countS++;
    double serial = omp_get_wtime() - t;

    t = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 100) reduction(+:countP)
    for (int i = 1; i <= n; i++)
        if (isPrime(i)) { flag[i] = 1; countP++; }
    double parallel = omp_get_wtime() - t;

    if (n <= 100) {
        printf("Primes: ");
        for (int i = 1; i <= n; i++) if (flag[i]) printf("%d ", i);
        printf("\n");
    }
    printf("Number of primes : %d (serial) / %d (parallel)\n", countS, countP);
    printf("Serial time      : %f s\n", serial);
    printf("Parallel time    : %f s\n", parallel);

    free(flag);
    return 0;
}
```

**Compile:**
```
gcc -fopenmp p4.c -o p4.exe
```

**Run:**
```
.\p4.exe
```
Sample input: `100000`

---

## Program 5: MPI_Send and MPI_Recv
File: `p5.c`

```c
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
```

**Compile:**
```
gcc p5.c -o p5.exe -I"%MSMPI_INC%" -L"%MSMPI_LIB64%" -lmsmpi
```

**Run:**
```
mpiexec -n 2 p5.exe
```

---

## Program 6: Deadlock and its avoidance (MPI)
File: `p6.c`

```c
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
```

**Compile:**
```
gcc p6.c -o p6.exe -I"%MSMPI_INC%" -L"%MSMPI_LIB64%" -lmsmpi
```

**Run:**
```
mpiexec -n 2 p6.exe 1     (deadlock - hangs, press Ctrl+C)
mpiexec -n 2 p6.exe 2     (deadlock avoided)
```

---

## Program 7: MPI_Bcast
File: `p7.c`

```c
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
```

**Compile:**
```
gcc p7.c -o p7.exe -I"%MSMPI_INC%" -L"%MSMPI_LIB64%" -lmsmpi
```

**Run:**
```
mpiexec -n 4 p7.exe
```

---

## Program 8: MPI_Scatter and MPI_Gather
File: `p8.c`

```c
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
```

**Compile:**
```
gcc p8.c -o p8.exe -I"%MSMPI_INC%" -L"%MSMPI_LIB64%" -lmsmpi
```

**Run:**
```
mpiexec -n 4 p8.exe
```

---

## Program 9: MPI_Reduce and MPI_Allreduce
File: `p9.c`

```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, value, max, min, sum, prod;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    value = rank + 1;
    printf("Process %d has value %d\n", rank, value);

    MPI_Reduce(&value, &max,  1, MPI_INT, MPI_MAX,  0, MPI_COMM_WORLD);
    MPI_Reduce(&value, &min,  1, MPI_INT, MPI_MIN,  0, MPI_COMM_WORLD);
    MPI_Reduce(&value, &sum,  1, MPI_INT, MPI_SUM,  0, MPI_COMM_WORLD);
    MPI_Reduce(&value, &prod, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("\n[MPI_Reduce at root] MAX=%d MIN=%d SUM=%d PROD=%d\n\n",
               max, min, sum, prod);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allreduce(&value, &max,  1, MPI_INT, MPI_MAX,  MPI_COMM_WORLD);
    MPI_Allreduce(&value, &min,  1, MPI_INT, MPI_MIN,  MPI_COMM_WORLD);
    MPI_Allreduce(&value, &sum,  1, MPI_INT, MPI_SUM,  MPI_COMM_WORLD);
    MPI_Allreduce(&value, &prod, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    printf("[MPI_Allreduce] Process %d: MAX=%d MIN=%d SUM=%d PROD=%d\n",
           rank, max, min, sum, prod);

    MPI_Finalize();
    return 0;
}
```

**Compile:**
```
gcc p9.c -o p9.exe -I"%MSMPI_INC%" -L"%MSMPI_LIB64%" -lmsmpi
```

**Run:**
```
mpiexec -n 4 p9.exe
```
