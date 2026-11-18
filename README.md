<<<<<<< HEAD
﻿# EV Charging Station Placement and Trip Planning System

## 1. Project Overview

The EV Charging Station Placement and Trip Planning System is a menu-driven application developed in C. It helps users explore routes between locations, calculate shortest-distance and fastest routes, and compare strategies for placing electric vehicle charging stations.

The system uses graph-based data structures and Design and Analysis of Algorithms (DAA) techniques.

## 2. Objectives

- Represent a road network using a weighted graph.
- Find shortest routes using Dijkstra's algorithm.
- Calculate shortest distances using Bellman-Ford.
- Find fastest routes using travel-time weights.
- Install charging stations manually or automatically.
- Compare connectivity-based and demand-based placement strategies.
- Demonstrate sorting, searching and large-data benchmarking.

## 3. Technologies Used

- Programming Language: C
- Compiler: GCC
- Standard: C11
- Development Environment: Visual Studio Code
- Data Structure: Weighted Graph using Adjacency Lists

## 4. Algorithms Implemented

| Algorithm | Purpose |
|---|---|
| Dijkstra | Shortest-distance route |
| Dijkstra with time weights | Fastest route |
| Bellman-Ford | Shortest-distance calculation |
| Greedy Algorithm | Automatic charging station placement |
| Quick Sort | Sorting values |
| Binary Search | Searching sorted values |
| Connectivity-Based Greedy | Charging station strategy |
| Demand-Based Greedy | Charging station strategy |

## 5. Project Features

1. Add new locations.
2. Add roads between locations.
3. Display the road network.
4. Find shortest-distance routes.
5. Find fastest routes.
6. Compare Dijkstra and Bellman-Ford.
7. Install charging stations manually.
8. Display installed charging stations.
9. Automatically place charging stations using a greedy strategy.
10. Perform Quick Sort and Binary Search.
11. Compare charging station placement strategies.
12. Run a large-data stress test.

## 6. Project Structure

- main.c – Main program and menu.
- graph.c / graph.h – Graph creation and management.
- dijkstra.c / dijkstra.h – Dijkstra's algorithm.
- fastest_route.c / fastest_route.h – Fastest route functionality.
- bellman_ford.c / bellman_ford.h – Bellman-Ford algorithm.
- ev_planner.c / ev_planner.h – Main application functions.
- input_validation.c / input_validation.h – Input validation.
- advanced_algorithms.c / advanced_algorithms.h – Sorting and searching.
- station_comparison.c / station_comparison.h – Charging station strategy comparison.
- stress_benchmark.c / stress_benchmark.h – Large-data benchmark.
- tests/test_cases.md – Test cases and results.
- data/sample_routes.csv – Sample route data.
- Makefile – Build configuration.

## 7. Compilation

Compile the project using GCC:

```bash
gcc -Wall -Wextra -std=c11 main.c graph.c dijkstra.c fastest_route.c bellman_ford.c ev_planner.c input_validation.c advanced_algorithms.c station_comparison.c stress_benchmark.c -o ev_planner
```

## 8. Execution

On Windows PowerShell:

```powershell
.\ev_planner.exe
```

On Linux:

```bash
./ev_planner
```

## 9. Test Scenarios

### Scenario 1: Normal Operation
Tests the predefined Dehradun road network and route-finding algorithms.

### Scenario 2: Capacity and Budget Constraints
Tests charging station placement under budget and charger-capacity restrictions and compares two placement strategies.

### Scenario 3: Large-Data Stress Test
Uses a synthetic network containing 50 locations and 184 road connections to compare Dijkstra and Bellman-Ford.

## 10. Benchmark Results

The synthetic stress test was executed for 5,000 repetitions.

| Parameter | Result |
|---|---:|
| Locations | 50 |
| Road connections | 184 |
| Dijkstra average time | 0.0128 ms |
| Bellman-Ford average time | 0.0136 ms |
| Correctness | Matching distances |

Execution times are dependent on hardware, compiler and system conditions.

## 11. Limitations

- Road distances and travel times are illustrative estimates.
- Demand scores are synthetic inputs.
- The system does not use live traffic or real-time charging station data.
- The graph has a maximum capacity of 50 locations.
- Benchmark memory values are structural estimates rather than measured peak memory usage.

## 12. Future Scope

- Integration with real map and traffic APIs.
- Use of real EV charging station datasets.
- Dynamic route planning based on battery level.
- More advanced charging station optimization.
- Graphical user interface and database integration.

## 13. Conclusion

The project demonstrates the application of graph algorithms, greedy strategies, sorting, searching and algorithm benchmarking in an electric vehicle trip-planning problem. The implemented scenarios demonstrate route calculation, charging station strategy comparison and shortest-path correctness on a synthetic large network.
=======

>>>>>>> origin/main
