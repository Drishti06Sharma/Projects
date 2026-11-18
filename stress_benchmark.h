
#ifndef STRESS_BENCHMARK_H
#define STRESS_BENCHMARK_H

typedef struct {
    int node_count;
    int edge_count;
    int repetitions;
    double dijkstra_time_ms;
    double bellman_time_ms;
    int distances_match;
    unsigned long graph_memory;
    unsigned long distance_array_memory;
    unsigned long dijkstra_visited_memory;
} StressBenchmarkResult;

int execute_stress_benchmark(int repetitions, StressBenchmarkResult *result);
void run_stress_benchmark(void);

#endif
