@echo off
title EV Trip Planner - Modern GUI Server
cd /d "%~dp0"

echo ============================================================
echo   EV TRIP PLANNER - GUI DASHBOARD
echo   CSEG3053: Design and Analysis of Algorithms
echo ============================================================
echo.

if not exist ev_server.exe (
    echo [INFO] ev_server.exe not found. Compiling now with GCC...
    gcc -Wall -Wextra -std=c11 ev_server.c graph.c dijkstra.c fastest_route.c bellman_ford.c advanced_algorithms.c station_comparison.c stress_benchmark.c -lws2_32 -o ev_server.exe
    if %errorlevel% neq 0 (
        echo [ERROR] Compilation failed. Please ensure GCC is installed and on your PATH.
        pause
        exit /b 1
    )
    echo [OK] Compiled ev_server.exe successfully.
)

echo [INFO] Starting C server on port 8080...
echo [INFO] Opening dashboard in your default browser...
start "" "http://localhost:8080/"

ev_server.exe 8080 --no-open

echo.
echo Server stopped.
pause
