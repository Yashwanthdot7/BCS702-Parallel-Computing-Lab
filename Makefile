# Linux / WSL build:  make   (binaries go to linux/)
CC = gcc
OMP = p1_mergesort p2_static_schedule p3_fibonacci_tasks p4_primes
MPI = p5_send_recv p6_deadlock p7_broadcast p8_scatter_gather p9_reduce_allreduce

all: $(addprefix linux/,$(OMP) $(MPI))

linux/%: src/%.c
	@mkdir -p linux
	$(if $(filter $*,$(MPI)),mpicc,$(CC) -fopenmp) -O2 $< -o $@

clean:
	rm -rf linux
