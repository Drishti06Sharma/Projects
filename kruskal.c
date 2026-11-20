#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "kruskal.h"

#define MAX_EDGES (MAX_NODES * MAX_NODES)

typedef struct
{
    int source;
    int destination;
    double weight;
} MSTEdge;

static int parent[MAX_NODES];
static int rank_array[MAX_NODES];

/* ---------- UNION FIND ---------- */

static void initialize_sets(int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        parent[i] = i;
        rank_array[i] = 0;
    }
}

static int find_parent(int node)
{
    if (parent[node] != node)
        parent[node] = find_parent(parent[node]);

    return parent[node];
}

static void union_sets(int a, int b)
{
    int root_a = find_parent(a);
    int root_b = find_parent(b);

    if (root_a == root_b)
        return;

    if (rank_array[root_a] < rank_array[root_b])
    {
        parent[root_a] = root_b;
    }
    else if (rank_array[root_a] > rank_array[root_b])
    {
        parent[root_b] = root_a;
    }
    else
    {
        parent[root_b] = root_a;
        rank_array[root_a]++;
    }
}

/* ---------- SORT EDGES ---------- */

static void sort_edges(MSTEdge edges[], int count)
{
    int i, j;
    MSTEdge temp;

    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - i - 1; j++)
        {
            if (edges[j].weight > edges[j + 1].weight)
            {
                temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

/* ---------- KRUSKAL ---------- */

void kruskal_mst(const Graph *graph)
{
    MSTEdge edges[MAX_EDGES];
    MSTEdge selected[MAX_NODES];

    int edge_count = 0;
    int selected_count = 0;
    int components;

    int i, j;

    double total = 0.0;

    clock_t start, end;
    double elapsed;

    if (graph == NULL || graph->node_count <= 0)
    {
        printf("Invalid graph.\n");
        return;
    }

    /* Collect each undirected road only once */
    for (i = 0; i < graph->node_count; i++)
    {
        for (j = 0; j < graph->nodes[i].edge_count; j++)
        {
            int destination =
                graph->nodes[i].edges[j].destination;

            if (i < destination)
            {
                if (edge_count >= MAX_EDGES)
                {
                    printf("Maximum edge capacity exceeded.\n");
                    return;
                }

                edges[edge_count].source = i;
                edges[edge_count].destination = destination;
                edges[edge_count].weight =
                    graph->nodes[i].edges[j].distance;

                edge_count++;
            }
        }
    }

    start = clock();

    sort_edges(edges, edge_count);

    initialize_sets(graph->node_count);

    components = graph->node_count;

    /* Select minimum-weight edges without cycles */
    for (i = 0; i < edge_count; i++)
    {
        int u = edges[i].source;
        int v = edges[i].destination;

        if (find_parent(u) != find_parent(v))
        {
            union_sets(u, v);

            selected[selected_count++] = edges[i];

            total += edges[i].weight;

            components--;

            if (selected_count == graph->node_count - 1)
                break;
        }
    }

    end = clock();

    elapsed = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("\n===== KRUSKAL'S ALGORITHM =====\n");

    for (i = 0; i < selected_count; i++)
    {
        printf("%-25s -> %-25s %.2f km\n",
               graph->nodes[selected[i].source].name,
               graph->nodes[selected[i].destination].name,
               selected[i].weight);
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