#include <float.h>
#include <stddef.h>
#include <math.h>

#include "dijkstra.h"

/* ---------- DIJKSTRA: SOURCE TO DESTINATION ---------- */

int dijkstra(const Graph *graph,
             int source,
             int destination,
             double distances[MAX_NODES],
             int previous[MAX_NODES])
{
    int visited[MAX_NODES] = {0};
    int i, count;

    /* Validate pointers */
    if (graph == NULL ||
        distances == NULL ||
        previous == NULL)
    {
        return 0;
    }

    /* Validate graph size */
    if (graph->node_count <= 0 ||
        graph->node_count > MAX_NODES)
    {
        return 0;
    }

    /* Validate source and destination */
    if (source < 0 || source >= graph->node_count ||
        destination < 0 || destination >= graph->node_count)
    {
        return 0;
    }

    /* Initialize distances and previous nodes */
    for (i = 0; i < graph->node_count; i++)
    {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    distances[source] = 0.0;

    /* Main Dijkstra algorithm */
    for (count = 0; count < graph->node_count; count++)
    {
        int current = -1;
        double minimum = DBL_MAX;

        /* Find unvisited node with minimum distance */
        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] &&
                distances[i] < minimum)
            {
                minimum = distances[i];
                current = i;
            }
        }

        /* No reachable node remains */
        if (current == -1)
            break;

        visited[current] = 1;

        /* Destination reached */
        if (current == destination)
            break;

        /* Relax adjacent edges */
        if (graph->nodes[current].edge_count < 0 ||
            graph->nodes[current].edge_count > MAX_NODES)
        {
            return 0;
        }

        for (i = 0;
             i < graph->nodes[current].edge_count;
             i++)
        {
            int next =
                graph->nodes[current].edges[i].destination;

            double distance =
                graph->nodes[current].edges[i].distance;

            /* Validate edge */
            if (next < 0 || next >= graph->node_count ||
                !isfinite(distance) || distance < 0)
            {
                return 0;
            }

            if (!visited[next] &&
                distances[current] + distance < distances[next])
            {
                distances[next] =
                    distances[current] + distance;

                previous[next] = current;
            }
        }
    }

    return 1;
}


/* ---------- DIJKSTRA: SOURCE TO ALL LOCATIONS ---------- */

int dijkstra_all(const Graph *graph,
                 int source,
                 double distances[MAX_NODES],
                 int previous[MAX_NODES])
{
    int visited[MAX_NODES] = {0};
    int i, count;

    /* Validate pointers */
    if (graph == NULL ||
        distances == NULL ||
        previous == NULL)
    {
        return 0;
    }

    /* Validate graph size */
    if (graph->node_count <= 0 ||
        graph->node_count > MAX_NODES)
    {
        return 0;
    }

    /* Validate source */
    if (source < 0 || source >= graph->node_count)
    {
        return 0;
    }

    /* Initialize arrays */
    for (i = 0; i < graph->node_count; i++)
    {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    distances[source] = 0.0;

    /* Main Dijkstra algorithm */
    for (count = 0; count < graph->node_count; count++)
    {
        int current = -1;
        double minimum = DBL_MAX;

        /* Find minimum unvisited distance */
        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] &&
                distances[i] < minimum)
            {
                minimum = distances[i];
                current = i;
            }
        }

        /* Stop if remaining nodes are unreachable */
        if (current == -1)
            break;

        visited[current] = 1;

        /* Validate edge count */
        if (graph->nodes[current].edge_count < 0 ||
            graph->nodes[current].edge_count > MAX_NODES)
        {
            return 0;
        }

        /* Relax adjacent edges */
        for (i = 0;
             i < graph->nodes[current].edge_count;
             i++)
        {
            int next =
                graph->nodes[current].edges[i].destination;

            double distance =
                graph->nodes[current].edges[i].distance;

            /* Validate edge */
            if (next < 0 || next >= graph->node_count ||
                !isfinite(distance) || distance < 0)
            {
                return 0;
            }

            if (!visited[next] &&
                distances[current] + distance < distances[next])
            {
                distances[next] =
                    distances[current] + distance;

                previous[next] = current;
            }
        }
    }

    return 1;
}