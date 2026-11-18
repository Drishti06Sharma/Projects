@echo off
title EV Trip Planner - Console Application
cd /d "%~dp0"

echo ============================================================
echo   EV TRIP PLANNER - CONSOLE APPLICATION
echo   CSEG3053: Design and Analysis of Algorithms
echo ============================================================
echo.

if not exist ev_planner.exe (
    echo [INFO] ev_planner.exe not found. Compiling now with GCC...
    gcc -Wall -Wextra -std=c11 main.c graph.c dijkstra.c fastest_route.c bellman_ford.c ev_planner.c input_validation.c advanced_algorithms.c station_comparison.c stress_benchmark.c -o ev_planner.exe
    if %errorlevel% neq 0 (
        echo [ERROR] Compilation failed. Please ensure GCC is installed and on your PATH.
        pause
        exit /b 1
    )
    echo [OK] Compiled ev_planner.exe successfully.
)

ev_planner.exe
pause
