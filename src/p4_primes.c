/* Program 4: primes from 1 to n, serial vs parallel for */
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
