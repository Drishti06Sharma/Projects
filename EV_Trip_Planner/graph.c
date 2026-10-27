
#include <stdio.h>
#include <string.h>
#include "graph.h"

void initialize_graph(Graph *graph)
{
    graph->node_count = 0;
}

int add_location(Graph *graph, const char *name)
{
    int i;

    if (graph->node_count >= MAX_NODES)
    {
        printf("Maximum locations reached!\n");
        return -1;
    }

    if (name == NULL || name[0] == '\0')
    {
        printf("Location name cannot be empty!\n");
        return -1;
    }

    for (i = 0; i < graph->node_count; i++)
    {
        if (strcmp(graph->nodes[i].name, name) == 0)
        {
            printf("Location already exists!\n");
            return -1;
        }
    }

    i = graph->node_count;

    strncpy(graph->nodes[i].name, name, MAX_NAME - 1);
    graph->nodes[i].name[MAX_NAME - 1] = '\0';

    graph->nodes[i].edge_count = 0;
    graph->node_count++;

    return i;
}

int add_road(Graph *graph, int source, int destination,
             double distance)
{
    int i;

    if (graph == NULL)
    {
        printf("Invalid graph!\n");
        return 0;
    }

    if (source < 0 || source >= graph->node_count ||
        destination < 0 || destination >= graph->node_count)
    {
        printf("Invalid location!\n");
        return 0;
    }

    if (source == destination || distance <= 0)
    {
        printf("Invalid road details!\n");
        return 0;
    }

    if (graph->nodes[source].edge_count >= MAX_NODES ||
        graph->nodes[destination].edge_count >= MAX_NODES)
    {
        printf("Cannot add more roads!\n");
        return 0;
    }

    /*
       Multiple roads between the same locations are allowed.
       Each road can have a different distance.
    */

    i = graph->nodes[source].edge_count;

    graph->nodes[source].edges[i].destination = destination;
    graph->nodes[source].edges[i].distance = distance;
    graph->nodes[source].edge_count++;

    i = graph->nodes[destination].edge_count;

    graph->nodes[destination].edges[i].destination = source;
    graph->nodes[destination].edges[i].distance = distance;
    graph->nodes[destination].edge_count++;

    return 1;
}

void display_locations(const Graph *graph)
{
    int i, j;

    if (graph == NULL || graph->node_count == 0)
    {
        printf("No locations available.\n");
        return;
    }

    printf("\n--- EV Road Network ---\n");

    for (i = 0; i < graph->node_count; i++)
    {
        printf("\nLocation %d: %s\n",
               i, graph->nodes[i].name);

        for (j = 0; j < graph->nodes[i].edge_count; j++)
        {
            int next = graph->nodes[i].edges[j].destination;

            printf("  -> %s (%.2f km)\n",
                   graph->nodes[next].name,
                   graph->nodes[i].edges[j].distance);
        }
    }
}
