#include <stdio.h>
#include <float.h>
#include <time.h>

#include "prim.h"
#include "kruskal.h"

/* ---------- PRIM'S ALGORITHM ---------- */

static void run_prim(const Graph *graph)
{
    int visited[MAX_NODES] = {0};

    int parent[MAX_NODES];
    double key[MAX_NODES];

    int selected_source[MAX_NODES];
    int selected_destination[MAX_NODES];

    double selected_weight[MAX_NODES];

    int selected_count = 0;
    int components = 0;

    int i, j;

    double total = 0.0;

    clock_t start, end;
    double elapsed;

    for (i = 0; i < graph->node_count; i++)
    {
        key[i] = DBL_MAX;
        parent[i] = -1;
    }

    start = clock();

    /* Handle disconnected graphs */
    for (int component_start = 0;
         component_start < graph->node_count;
         component_start++)
    {
        if (visited[component_start])
            continue;

        components++;

        key[component_start] = 0.0;

        /* Process current component */
        while (1)
        {
            int current = -1;
            double minimum = DBL_MAX;

            /* Select minimum key unvisited vertex */
            for (i = 0; i < graph->node_count; i++)
            {
                if (!visited[i] && key[i] < minimum)
                {
                    minimum = key[i];
                    current = i;
                }
            }

            if (current == -1)
                break;

            visited[current] = 1;

            /* Store selected edge */
            if (parent[current] != -1)
            {
                selected_source[selected_count] =
                    parent[current];

                selected_destination[selected_count] =
                    current;

                selected_weight[selected_count] =
                    key[current];

                total += key[current];

                selected_count++;
            }

            /* Update neighboring vertices */
            for (j = 0;
                 j < graph->nodes[current].edge_count;
                 j++)
            {
                int next =
                    graph->nodes[current].edges[j].destination;

                double weight =
                    graph->nodes[current].edges[j].distance;

                if (!visited[next] && weight < key[next])
                {
                    key[next] = weight;
                    parent[next] = current;
                }
            }
        }
    }

    end = clock();

    elapsed = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("\n===== PRIM'S ALGORITHM =====\n");

    for (i = 0; i < selected_count; i++)
    {
        printf("%-25s -> %-25s %.2f km\n",
               graph->nodes[selected_source[i]].name,
               graph->nodes[selected_destination[i]].name,
               selected_weight[i]);
    }

    printf("Edges selected: %d\n", selected_count);
    printf("Total distance: %.2f km\n", total);
    printf("Components: %d\n", components);
    printf("Execution time: %.6f ms\n", elapsed);

    if (components == 1)
        printf("Minimum Spanning Tree completed.\n");
    else
        printf("Graph disconnected. Minimum Spanning Forest generated.\n");
}

/* ---------- MST COMPARISON ---------- */

void compare_mst(Graph *graph)
{
    if (graph == NULL || graph->node_count <= 0)
    {
        printf("Invalid graph.\n");
        return;
    }

    printf("\n===== MST COMPARISON =====\n");
    printf("Vertices: %d\n", graph->node_count);

    run_prim(graph);

    kruskal_mst(graph);

    printf("\n===== MST COMPARISON COMPLETED =====\n");
}