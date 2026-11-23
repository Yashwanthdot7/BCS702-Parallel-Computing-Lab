# BCS702 – Parallel Computing Lab (VTU 2022 Scheme)

All 9 programs (OpenMP + MPI), prebuilt for Windows. Clone and run.

## Quick start (Windows)

```bat
git clone https://github.com/Yashwanthdot7/BCS702-Parallel-Computing-Lab.git
cd BCS702-Parallel-Computing-Lab
.\run.bat
```

Choose a program number from the menu. Or run one directly: `.\run.bat 5`.

**Requirements**
- Programs 1–4 (OpenMP): nothing. The `.exe` files are statically linked.
- Programs 5–9 (MPI): the **MS-MPI runtime** (`msmpisetup.exe`), which provides `mpiexec` and `msmpi.dll`. Lab PCs set up for BCS702 already have it.

If a prebuilt `.exe` doesn't run, rebuild everything on that PC:

```bat
.\build.bat
```

`build.bat` needs gcc (CodeBlocks MinGW works; it's detected automatically) and the MS-MPI SDK (`msmpisdk.msi`).

## Programs

| # | Program | Source | Run |
|---|---------|--------|-----|
| 1 | Merge sort – sequential vs parallel (sections) | `src/p1_mergesort.c` | `bin\p1_mergesort.exe` |
| 2 | `static,2` scheduling – which thread runs which iterations | `src/p2_static_schedule.c` | `bin\p2_static_schedule.exe` |
| 3 | Fibonacci using tasks | `src/p3_fibonacci_tasks.c` | `bin\p3_fibonacci_tasks.exe` |
| 4 | Primes 1..n – serial vs `parallel for` | `src/p4_primes.c` | `bin\p4_primes.exe` |
| 5 | `MPI_Send` / `MPI_Recv` | `src/p5_send_recv.c` | `mpiexec -n 2 bin\p5_send_recv.exe` |
| 6 | Deadlock and its avoidance | `src/p6_deadlock.c` | `mpiexec -n 2 bin\p6_deadlock.exe 1` (hangs) / `... 2` |
| 7 | `MPI_Bcast` | `src/p7_broadcast.c` | `mpiexec -n 4 bin\p7_broadcast.exe` |
| 8 | `MPI_Scatter` / `MPI_Gather` | `src/p8_scatter_gather.c` | `mpiexec -n 4 bin\p8_scatter_gather.exe` |
| 9 | `MPI_Reduce` / `MPI_Allreduce` (MAX, MIN, SUM, PROD) | `src/p9_reduce_allreduce.c` | `mpiexec -n 4 bin\p9_reduce_allreduce.exe` |

## Sample outputs

Timings and line order vary between runs and machines.

**P1** (n = 1000000)
```
Sequential time : 0.232704 s
Parallel time   : 0.157011 s
Difference      : 0.075692 s
```

**P2** (8 iterations, 4 threads)
```
Thread 0 : Iterations 0 -- 1
Thread 1 : Iterations 2 -- 3
Thread 2 : Iterations 4 -- 5
Thread 3 : Iterations 6 -- 7
```

**P3** (n = 15)
```
Fibonacci series: 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377
```

**P4** (n = 50; use n = 100000 to see the parallel speed-up)
```
Primes: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47
Number of primes : 15 (serial) / 15 (parallel)
```

**P5**
```
Process 0 sent 100 to process 1
Process 1 received 100 from process 0
```

**P6**
```
PART 1 (hangs - press Ctrl+C):
Process 0 waiting to receive from Process 1...
Process 1 waiting to receive from Process 0...

PART 2:
Process 0 received back number: 123
Process 1 received and sent back number: 123
```

**P7**
```
Process 0 broadcasting value 50
Process 0 received value 50
Process 1 received value 50
Process 2 received value 50
Process 3 received value 50
```

**P8**
```
Root array: 1 2 3 4
Process 0 received 1
Process 1 received 2
Process 2 received 3
Process 3 received 4
Gathered (squared) array: 1 4 9 16
```

**P9**
```
[MPI_Reduce at root] MAX=4 MIN=1 SUM=10 PROD=24
[MPI_Allreduce] Process 0: MAX=4 MIN=1 SUM=10 PROD=24
... (same for every process)
```

## Linux / WSL

```bash
make            # builds into linux/
./linux/p1_mergesort
mpirun -n 4 ./linux/p7_broadcast
```
