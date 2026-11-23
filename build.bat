@echo off
REM Rebuild all programs on Windows (only needed if you edit the code).
REM Needs: gcc (MinGW / CodeBlocks MinGW) and the MS-MPI SDK (msmpisdk.msi).
setlocal enabledelayedexpansion
cd /d "%~dp0"

REM Use CodeBlocks' MinGW if gcc is not already on PATH
where gcc >nul 2>nul
if errorlevel 1 (
    if exist "C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe" set "PATH=C:\Program Files\CodeBlocks\MinGW\bin;!PATH!"
    if exist "C:\Program Files (x86)\CodeBlocks\MinGW\bin\gcc.exe" set "PATH=C:\Program Files (x86)\CodeBlocks\MinGW\bin;!PATH!"
)
where gcc >nul 2>nul
if errorlevel 1 (
    echo ERROR: gcc not found. Install CodeBlocks with MinGW, or add MinGW\bin to PATH.
    exit /b 1
)

if not exist bin mkdir bin

echo Building OpenMP programs 1-4...
for %%P in (p1_mergesort p2_static_schedule p3_fibonacci_tasks p4_primes) do (
    gcc -O2 -fopenmp src\%%P.c -o bin\%%P.exe -static
    if errorlevel 1 (echo   FAILED: %%P) else (echo   OK: %%P)
)

REM Pick 64-bit or 32-bit MS-MPI library to match the compiler
set "MPILIB=%MSMPI_LIB64%"
for /f %%M in ('gcc -dumpmachine') do set "GCCM=%%M"
echo !GCCM! | find "x86_64" >nul || set "MPILIB=%MSMPI_LIB32%"

if "%MSMPI_INC%"=="" (
    echo ERROR: MS-MPI SDK not found ^(MSMPI_INC not set^). Install msmpisdk.msi and reopen the terminal.
    exit /b 1
)

echo Building MPI programs 5-9...
for %%P in (p5_send_recv p6_deadlock p7_broadcast p8_scatter_gather p9_reduce_allreduce) do (
    gcc -O2 src\%%P.c -o bin\%%P.exe -I"%MSMPI_INC%" -I"%MSMPI_INC%\x64" -L"!MPILIB!" -lmsmpi
    if errorlevel 1 (echo   FAILED: %%P) else (echo   OK: %%P)
)
echo Done. Now run: run.bat
