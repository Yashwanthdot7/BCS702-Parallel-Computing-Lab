/* Program 2: static scheduling with chunk size 2 (OMP_SCHEDULE=static,2)
 * Prints which iterations each thread executed, chunk by chunk. */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter number of iterations: ");
    scanf("%d", &n);

    int *owner = malloc(n * sizeof(int));   /* owner[i] = thread that ran iteration i */

    omp_set_schedule(omp_sched_static, 2);   /* same as OMP_SCHEDULE=static,2 */

    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < n; i++) {
        owner[i] = omp_get_thread_num();
    }

    /* print each run of consecutive iterations done by the same thread */
    for (int i = 0; i < n; ) {
        int start = i;
        while (i + 1 < n && owner[i + 1] == owner[start] && (i + 1 - start) < 2) i++;
        printf("Thread %d : Iterations %d -- %d\n", owner[start], start, i);
        i++;
    }

    free(owner);
    return 0;
}
