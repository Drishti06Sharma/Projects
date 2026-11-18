
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <math.h>

#include "graph.h"
#include "dijkstra.h"
#include "bellman_ford.h"
#include "stress_benchmark.h"

#define STRESS_NODES 50
#define REPETITIONS 5000

static int create_stress_graph(Graph *graph)
{
    char name[MAX_NAME];
    int i, j;

    graph->node_count = 0;

    /* Create 50 locations. */
    for (i = 0; i < STRESS_NODES; i++)
    {
        snprintf(name, sizeof(name), "Stress_Node_%02d", i);

        if (add_location(graph, name) < 0)
            return 0;
    }

    /* Create a connected network with deterministic weights. */
    for (i = 0; i < STRESS_NODES; i++)
    {
        for (j = 1; j <= 7; j += 2)
        {
            int next = i + j;

            if (next < STRESS_NODES)
            {
                double distance =
                    1.0 + ((i * 7 + next * 3) % 20);

                double travel_time = distance / 40.0;

                if (!add_road(graph, i, next,
                              distance, travel_time))
                    return 0;
            }
        }
    }

    return 1;
}

int execute_stress_benchmark(int repetitions, StressBenchmarkResult *result)
{
    Graph test_graph = {0};

    double dijkstra_distances[MAX_NODES];
    double bellman_distances[MAX_NODES];

    int dijkstra_previous[MAX_NODES];
    int bellman_previous[MAX_NODES];

    int i, result_d, result_b;
    int edge_count = 0;
    int distances_match = 1;

    clock_t start, end;
    double dijkstra_time, bellman_time;

    if (repetitions <= 0)
        repetitions = REPETITIONS;

    if (!create_stress_graph(&test_graph))
    {
        return 0;
    }

    for (i = 0; i < test_graph.node_count; i++)
        edge_count += test_graph.nodes[i].edge_count;

    edge_count /= 2;

    /* Benchmark Dijkstra. */
    start = clock();

    for (i = 0; i < repetitions; i++)
    {
        result_d = dijkstra_all(
            &test_graph, 0,
            dijkstra_distances,
            dijkstra_previous
        );

        if (!result_d)
        {
            return 0;
        }
    }

    end = clock();

    dijkstra_time =
        ((double)(end - start) / CLOCKS_PER_SEC)
        * 1000.0 / repetitions;

    /* Benchmark Bellman-Ford. */
    start = clock();

    for (i = 0; i < repetitions; i++)
    {
        result_b = bellman_ford(
            &test_graph, 0,
            bellman_distances,
            bellman_previous
        );

        if (result_b != 1)
        {
            return 0;
        }
    }

    end = clock();

    bellman_time =
        ((double)(end - start) / CLOCKS_PER_SEC)
        * 1000.0 / repetitions;

    /* Compare shortest distances from both algorithms. */
    for (i = 0; i < test_graph.node_count; i++)
    {
        if (fabs(dijkstra_distances[i] -
                 bellman_distances[i]) > 0.000001)
        {
            distances_match = 0;
            break;
        }
    }

    if (result != NULL)
    {
        result->node_count = test_graph.node_count;
        result->edge_count = edge_count;
        result->repetitions = repetitions;
        result->dijkstra_time_ms = dijkstra_time;
        result->bellman_time_ms = bellman_time;
        result->distances_match = distances_match;
        result->graph_memory = (unsigned long)sizeof(Graph);
        result->distance_array_memory = (unsigned long)(sizeof(double) * MAX_NODES + sizeof(int) * MAX_NODES);
        result->dijkstra_visited_memory = (unsigned long)(sizeof(int) * MAX_NODES);
    }

    return 1;
}

void run_stress_benchmark(void)
{
    StressBenchmarkResult res;

    printf("\n====================================\n");
    printf("       LARGE DATA STRESS TEST\n");
    printf("====================================\n");

    if (!execute_stress_benchmark(REPETITIONS, &res))
    {
        printf("Could not complete the stress benchmark.\n");
        return;
    }

    printf("\nSynthetic network created successfully.\n");
    printf("Locations: %d\n", res.node_count);
    printf("Road connections: %d\n", res.edge_count);
    printf("Source location: 0\n");
    printf("Algorithm repetitions: %d\n", res.repetitions);

    printf("\n--- EXECUTION TIME COMPARISON ---\n");
    printf("Dijkstra average time: %.6f ms\n", res.dijkstra_time_ms);
    printf("Bellman-Ford average time: %.6f ms\n", res.bellman_time_ms);

    printf("\n--- MEMORY ESTIMATION ---\n");
    printf("Graph structure memory: %lu bytes\n", res.graph_memory);
    printf("Common distance and previous arrays: %lu bytes\n", res.distance_array_memory);
    printf("Dijkstra visited array: %lu bytes\n", res.dijkstra_visited_memory);
    printf("Bellman-Ford additional per-node array: 0 bytes\n");

    printf("\n--- CORRECTNESS VERIFICATION ---\n");

    if (res.distances_match)
        printf("PASS: Both algorithms produced matching distances.\n");
    else
        printf("FAIL: Distance results do not match.\n");

    printf("\n--- TIME COMPLEXITY ---\n");
    printf("Dijkstra: O(V^2 + E)\n");
    printf("Bellman-Ford: O(V * E)\n");

    printf("\nNote: This is a synthetic benchmark.\n");
    printf("Execution times are average CPU times.\n");
    printf("Memory values are structural estimates, not measured peak RAM.\n");
    printf("Original Dehradun network remains unchanged.\n");

    printf("\n====================================\n");
}


