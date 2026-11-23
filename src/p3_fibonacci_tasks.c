/* Program 3: n Fibonacci numbers using OpenMP tasks */
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
