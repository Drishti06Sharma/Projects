#include <float.h>
#include <stddef.h>
#include <math.h>

#include "fastest_route.h"

/* ---------- FASTEST ROUTE USING DIJKSTRA ---------- */

int fastest_route(const Graph *graph,
                  int source,
                  int destination,
                  double times[MAX_NODES],
                  int previous[MAX_NODES])
{
    int visited[MAX_NODES] = {0};
    int i, count;

    /* Validate pointers */
    if (graph == NULL ||
        times == NULL ||
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

    /* Initialize arrays */
    for (i = 0; i < graph->node_count; i++)
    {
        times[i] = DBL_MAX;
        previous[i] = -1;
    }

    times[source] = 0.0;

    /* Main Dijkstra algorithm */
    for (count = 0; count < graph->node_count; count++)
    {
        int current = -1;
        double minimum = DBL_MAX;

        /* Find unvisited node with minimum travel time */
        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] &&
                times[i] < minimum)
            {
                minimum = times[i];
                current = i;
            }
        }

        /* No reachable location remains */
        if (current == -1)
            break;

        visited[current] = 1;

        /* Destination reached */
        if (current == destination)
            break;

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

            double travel_time =
                graph->nodes[current].edges[i].time;

            /* Validate edge */
            if (next < 0 || next >= graph->node_count ||
                !isfinite(travel_time) ||
                travel_time < 0)
            {
                return 0;
            }

            if (!visited[next] &&
                times[current] + travel_time < times[next])
            {
                times[next] =
                    times[current] + travel_time;

                previous[next] = current;
            }
        }
    }

    return 1;
}