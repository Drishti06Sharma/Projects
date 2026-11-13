
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <math.h>

#include "graph.h"
#include "dijkstra.h"
#include "bellman_ford.h"

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

void run_stress_benchmark(void)
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

    printf("\n====================================\n");
    printf("       LARGE DATA STRESS TEST\n");
    printf("====================================\n");

    if (!create_stress_graph(&test_graph))
    {
        printf("Could not create the test network.\n");
        return;
    }

    for (i = 0; i < test_graph.node_count; i++)
        edge_count += test_graph.nodes[i].edge_count;

    edge_count /= 2;

    printf("\nSynthetic network created successfully.\n");
    printf("Locations: %d\n", test_graph.node_count);
    printf("Road connections: %d\n", edge_count);
    printf("Source location: 0\n");
    printf("Algorithm repetitions: %d\n", REPETITIONS);

    /* Benchmark Dijkstra. */
    start = clock();

    for (i = 0; i < REPETITIONS; i++)
    {
        result_d = dijkstra_all(
            &test_graph, 0,
            dijkstra_distances,
            dijkstra_previous
        );

        if (!result_d)
        {
            printf("Dijkstra execution failed.\n");
            return;
        }
    }

    end = clock();

    dijkstra_time =
        ((double)(end - start) / CLOCKS_PER_SEC)
        * 1000.0 / REPETITIONS;

    /* Benchmark Bellman-Ford. */
    start = clock();

    for (i = 0; i < REPETITIONS; i++)
    {
        result_b = bellman_ford(
            &test_graph, 0,
            bellman_distances,
            bellman_previous
        );

        if (result_b != 1)
        {
            printf("Bellman-Ford execution failed.\n");
            return;
        }
    }

    end = clock();

    bellman_time =
        ((double)(end - start) / CLOCKS_PER_SEC)
        * 1000.0 / REPETITIONS;

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

    printf("\n--- EXECUTION TIME COMPARISON ---\n");
    printf("Dijkstra average time: %.6f ms\n", dijkstra_time);
    printf("Bellman-Ford average time: %.6f ms\n", bellman_time);

    printf("\n--- MEMORY ESTIMATION ---\n");
    printf("Graph structure memory: %lu bytes\n",
           (unsigned long)sizeof(Graph));

    printf("Common distance and previous arrays: %lu bytes\n",
           (unsigned long)(
               sizeof(double) * MAX_NODES +
               sizeof(int) * MAX_NODES
           ));

    printf("Dijkstra visited array: %lu bytes\n",
           (unsigned long)(sizeof(int) * MAX_NODES));

    printf("Bellman-Ford additional per-node array: 0 bytes\n");

    printf("\n--- CORRECTNESS VERIFICATION ---\n");

    if (distances_match)
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


