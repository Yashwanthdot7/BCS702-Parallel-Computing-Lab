/* Program 1: Sequential vs parallel mergesort (OpenMP sections) */
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
    parMergeSort(b, 0, n - 1, 3);   /* parallelise top 3 levels */
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
