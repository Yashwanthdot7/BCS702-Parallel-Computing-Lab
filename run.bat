@echo off
REM BCS702 Parallel Computing Lab - run any program 1 to 9
REM Usage: run.bat            (shows a menu)
REM        run.bat 5          (runs program 5 directly)
setlocal
cd /d "%~dp0"

set NP=4
if not "%~2"=="" set NP=%~2

set CH=%~1
if not "%CH%"=="" goto run

:menu
echo.
echo ===== BCS702 Parallel Computing Lab =====
echo  1. Merge sort - sequential vs parallel (OpenMP sections)
echo  2. Static schedule, chunk size 2 (OpenMP)
echo  3. Fibonacci using tasks (OpenMP)
echo  4. Prime numbers 1 to n - serial vs parallel (OpenMP)
echo  5. MPI_Send and MPI_Recv
echo  6. Deadlock and its avoidance (MPI)
echo  7. Broadcast (MPI)
echo  8. MPI_Scatter and MPI_Gather
echo  9. MPI_Reduce and MPI_Allreduce
echo  0. Exit
set /p CH=Choose program (0-9):

:run
if "%CH%"=="0" goto :eof
if "%CH%"=="1" bin\p1_mergesort.exe
if "%CH%"=="2" bin\p2_static_schedule.exe
if "%CH%"=="3" bin\p3_fibonacci_tasks.exe
if "%CH%"=="4" bin\p4_primes.exe
if "%CH%"=="5" mpiexec -n 2 bin\p5_send_recv.exe
if "%CH%"=="6" (
    echo.
    echo --- PART 1: Deadlock. The program will HANG. Press Ctrl+C to stop it. ---
    mpiexec -n 2 bin\p6_deadlock.exe 1
    echo.
    echo --- PART 2: Deadlock avoided by changing the call order ---
    mpiexec -n 2 bin\p6_deadlock.exe 2
)
if "%CH%"=="7" mpiexec -n %NP% bin\p7_broadcast.exe
if "%CH%"=="8" mpiexec -n %NP% bin\p8_scatter_gather.exe
if "%CH%"=="9" mpiexec -n %NP% bin\p9_reduce_allreduce.exe
if not "%~1"=="" goto :eof
set CH=
goto menu
