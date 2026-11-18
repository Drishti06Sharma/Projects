@echo off
title Build All - EV Trip Planner
cd /d "%~dp0"

echo ============================================================
echo   BUILDING EV TRIP PLANNER
echo   1. Console Application (ev_planner.exe)
echo   2. Native GUI Server (ev_server.exe)
echo ============================================================
echo.

echo [1/2] Compiling ev_planner.exe...
gcc -Wall -Wextra -std=c11 main.c graph.c dijkstra.c fastest_route.c bellman_ford.c ev_planner.c input_validation.c advanced_algorithms.c station_comparison.c stress_benchmark.c -o ev_planner.exe
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile ev_planner.exe
    pause
    exit /b %errorlevel%
)
echo [OK] ev_planner.exe built successfully.
echo.

echo [2/2] Compiling ev_server.exe...
gcc -Wall -Wextra -std=c11 ev_server.c graph.c dijkstra.c fastest_route.c bellman_ford.c advanced_algorithms.c station_comparison.c stress_benchmark.c -lws2_32 -o ev_server.exe
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile ev_server.exe
    pause
    exit /b %errorlevel%
)
echo [OK] ev_server.exe built successfully.
echo.

echo ============================================================
echo   ALL TARGETS BUILT CLEANLY AND READY TO RUN!
echo ============================================================
pause
