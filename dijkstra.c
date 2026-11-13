
#include <float.h>
#include <stddef.h>
#include "dijkstra.h"

/* Dijkstra: shortest distance between source and destination */
int dijkstra(const Graph *graph, int source, int destination,
             double distances[MAX_NODES],
             int previous[MAX_NODES])
{
    int visited[MAX_NODES] = {0};
    int i, count;

    if (graph == NULL ||
        distances == NULL || previous == NULL ||
        source < 0 || source >= graph->node_count ||
        destination < 0 || destination >= graph->node_count)
    {
        return 0;
    }

    for (i = 0; i < graph->node_count; i++)
    {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    distances[source] = 0.0;

    for (count = 0; count < graph->node_count; count++)
    {
        int current = -1;
        double minimum = DBL_MAX;

        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] && distances[i] < minimum)
            {
                minimum = distances[i];
                current = i;
            }
        }

        if (current == -1)
            break;

        visited[current] = 1;

        if (current == destination)
            break;

        for (i = 0; i < graph->nodes[current].edge_count; i++)
        {
            int next = graph->nodes[current].edges[i].destination;
            double distance = graph->nodes[current].edges[i].distance;

            if (!visited[next] &&
                distances[current] + distance < distances[next])
            {
                distances[next] = distances[current] + distance;
                previous[next] = current;
            }
        }
    }

    return 1;
}

/* Dijkstra: shortest distances from one source to all locations */
int dijkstra_all(const Graph *graph, int source,
                 double distances[MAX_NODES],
                 int previous[MAX_NODES])
{
    int visited[MAX_NODES] = {0};
    int i, count;

    if (graph == NULL ||
        distances == NULL || previous == NULL ||
        source < 0 || source >= graph->node_count)
    {
        return 0;
    }

    for (i = 0; i < graph->node_count; i++)
    {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    distances[source] = 0.0;

    for (count = 0; count < graph->node_count; count++)
    {
        int current = -1;
        double minimum = DBL_MAX;

        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] && distances[i] < minimum)
            {
                minimum = distances[i];
                current = i;
            }
        }

        if (current == -1)
            break;

        visited[current] = 1;

        for (i = 0; i < graph->nodes[current].edge_count; i++)
        {
            int next = graph->nodes[current].edges[i].destination;
            double distance = graph->nodes[current].edges[i].distance;

            if (!visited[next] &&
                distances[current] + distance < distances[next])
            {
                distances[next] = distances[current] + distance;
                previous[next] = current;
            }
        }
    }

    return 1;
}

