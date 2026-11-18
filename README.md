# EV Charging Station Placement and Trip Planning System

A comprehensive, graph-based algorithmic solution for electric vehicle travel planning, charging station infrastructure placement, and routing optimization developed for **Design and Analysis of Algorithms (DAA)**.

---

## 1. Project Overview

The **EV Trip Planner** models real-world electric mobility challenges using graph-based data structures and classical algorithmic paradigms. It provides:
1. **Interactive Modern EV Dashboard GUI:** A dark navy interface with electric cyan and blue neon accents, real-time Dehradun road network map rendering, interactive routing, live strategy comparison, sorting visualizer, and live stress testing.
2. **Terminal Console Interface:** A menu-driven C application supporting all 12 analytical operations.
3. **Pure C Backend Engine:** High-performance, memory-efficient C11 implementation with zero third-party dependencies.

---

## 2. Key Modules & DAA Paradigms

| Module / Feature | Algorithm | DAA Paradigm | Time Complexity | Space Complexity |
|---|---|---|---|---|
| **Shortest Distance Route** | Dijkstra's Algorithm | Greedy (Min-distance vertex selection) | $O(V^2 + E)$ | $O(V)$ |
| **Fastest Route** | Travel-Time Weighted Dijkstra | Greedy (Min-time vertex selection) | $O(V^2 + E)$ | $O(V)$ |
| **Route & Negative Cycle Verification** | Bellman-Ford Algorithm | Dynamic Programming (Repeated Relaxation) | $O(V \cdot E)$ | $O(V)$ |
| **Station Placement (Connectivity)** | Degree-Greedy Heuristic | Greedy Approximation ($\max \text{deg}(v)$) | $O(V \cdot k)$ | $O(V)$ |
| **Station Placement (Demand)** | Demand-Greedy Heuristic | Greedy Approximation ($\max \text{demand}(v)$) | $O(V \cdot k)$ | $O(V)$ |
| **Array Sorting** | Quick Sort | Divide and Conquer (Lomuto Partitioning) | $O(N \log N)$ avg | $O(\log N)$ aux |
| **Alternative Sorting** | Merge Sort | Divide and Conquer (Merge Split) | $O(N \log N)$ | $O(N)$ aux |
| **Fast Searching** | Binary Search (Iterative & Recursive) | Decrease and Conquer | $O(\log N)$ | $O(1)$ iter / $O(\log N)$ rec |
| **Stress Benchmark** | Synthetic Graph (50 nodes, 184 roads) | Empirical Complexity & Correctness | 5,000 reps | $O(V + E)$ |

---

## 3. Predefined Dehradun Road Network

The system preloads 10 prominent landmarks across Dehradun, Uttarakhand with realistic distance and estimated travel-time weights:

1. **Location 0:** Clock Tower (City Center)
2. **Location 1:** ISBT Dehradun (South-West)
3. **Location 2:** Railway Station (South)
4. **Location 3:** Forest Research Institute (FRI) (West)
5. **Location 4:** Pacific Mall (North-East, Rajpur Road)
6. **Location 5:** Prem Nagar (Far West)
7. **Location 6:** DIT University (Far North Foothills)
8. **Location 7:** Max Hospital (North-East)
9. **Location 8:** Survey Chowk (East of Clock Tower)
10. **Location 9:** Sahastradhara Road (East)

*Road connections include 16 bidirectional weighted segments linking the city's key arterials.*

---

## 4. Architecture & GUI Integration

```
┌────────────────────────────────────────────────────────┐
│             Modern Browser Dashboard (GUI)             │
│   (HTML5 Canvas, CSS3 Neon Dark Navy, JS Controllers)   │
└──────────────────────────┬─────────────────────────────┘
                           │ HTTP REST JSON Requests
                           ▼
┌────────────────────────────────────────────────────────┐
│           Native C Backend Server (ev_server.exe)       │
│    Built with C11 + Winsock2 (ws2_32) - Zero Python/Node│
└──────────────────────────┬─────────────────────────────┘
                           │ Direct In-Memory Function Calls
                           ▼
┌────────────────────────────────────────────────────────┐
│                   Core C DAA Algorithms                 │
│  • dijkstra.c          • fastest_route.c               │
│  • bellman_ford.c      • station_comparison.c          │
│  • advanced_algorithms.c • stress_benchmark.c          │
│  • graph.c (Adjacency-List Weighted Topology)          │
└────────────────────────────────────────────────────────┘
```

**Why this architecture is ideal for a DAA Viva:**
- **No Mock or Fake Results:** Every route distance, time, strategy selection, sort array, and stress benchmark execution is computed directly by the compiled C code.
- **Zero Heavy Dependencies:** Runs locally on any Windows PC using GCC and Windows Sockets with no need for Node.js, Python, or npm.
- **Dual Interface:** Both the modern web GUI and the classic console CLI share the exact same underlying C algorithmic files.

---

## 5. Quick Start & Execution (Windows)

### Option A: Launch Modern GUI (One-Click)
Simply double-click:
```cmd
run_gui.bat
```
*This automatically compiles `ev_server.exe` (if not already compiled), starts the local C server on port 8080, and opens your default web browser.*

### Option B: Run Classic Console Menu
Simply double-click:
```cmd
run_cli.bat
```
*Or run in PowerShell:*
```powershell
.\ev_planner.exe
```

### Option C: Rebuild All Executables
Double-click:
```cmd
build_all.bat
```
*Or via Makefile:*
```cmd
make all
```

---

## 6. Manual Compilation Commands (GCC)

If compiling manually using GCC on Windows PowerShell or CMD:

```powershell
# 1. Compile the Console Application:
gcc -Wall -Wextra -std=c11 main.c graph.c dijkstra.c fastest_route.c bellman_ford.c ev_planner.c input_validation.c advanced_algorithms.c station_comparison.c stress_benchmark.c -o ev_planner.exe

# 2. Compile the Native GUI Server:
gcc -Wall -Wextra -std=c11 ev_server.c graph.c dijkstra.c fastest_route.c bellman_ford.c advanced_algorithms.c station_comparison.c stress_benchmark.c -lws2_32 -o ev_server.exe
```

---

## 7. DAA Viva Voce Defense Guide

1. **How does Dijkstra handle fastest route vs shortest distance?**
   - The same priority-queue / greedy relaxation logic applies: for shortest distance, edge weights represent Euclidean distance ($d_{uv}$ in km); for fastest route, edge weights represent travel time ($t_{uv}$ in hours).
2. **Why does Bellman-Ford detect negative cycles while Dijkstra cannot?**
   - Dijkstra assumes that adding an edge to a path always increases its cost (greedy property holds only for non-negative weights). Bellman-Ford performs $V - 1$ global relaxations of all edges, guaranteeing convergence for any cycle-free path. A further relaxation on pass $V$ identifies edges that can still be minimized, proving a negative-weight cycle is reachable.
3. **What is the complexity of station strategy comparison?**
   - Each greedy iteration scans up to $V$ candidate nodes to find the maximum degree or demand score, giving $O(k \cdot V)$ where $k \le V$ is the number of stations allowed under budget.
4. **Empirical Benchmarking Results (Scenario 3):**
   - On a synthetic graph of 50 locations and 184 roads across 5,000 iterations:
     - Dijkstra average time: $\approx 0.012$ ms
     - Bellman-Ford average time: $\approx 0.013$ ms
     - Shortest distances match 100% across all pairs.
