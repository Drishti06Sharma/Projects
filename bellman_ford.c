#include <float.h>
#include <stddef.h>
#include <math.h>

#include "bellman_ford.h"

/* ---------- BELLMAN-FORD ALGORITHM ---------- */

int bellman_ford(const Graph *graph,
                 int source,
                 double distances[MAX_NODES],
                 int previous[MAX_NODES])
{
    int i, j, k;

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
    if (source < 0 ||
        source >= graph->node_count)
    {
        return 0;
    }

    /* Initialize distances and previous locations */
    for (i = 0; i < graph->node_count; i++)
    {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    distances[source] = 0.0;

    /* Relax all edges V - 1 times */
    for (k = 0; k < graph->node_count - 1; k++)
    {
        int updated = 0;

        for (i = 0; i < graph->node_count; i++)
        {
            /* Skip unreachable nodes */
            if (distances[i] == DBL_MAX)
                continue;

            /* Validate edge count */
            if (graph->nodes[i].edge_count < 0 ||
                graph->nodes[i].edge_count > MAX_NODES)
            {
                return 0;
            }

            for (j = 0;
                 j < graph->nodes[i].edge_count;
                 j++)
            {
                int next =
                    graph->nodes[i].edges[j].destination;

                double distance =
                    graph->nodes[i].edges[j].distance;

                /* Validate edge */
                if (next < 0 ||
                    next >= graph->node_count ||
                    !isfinite(distance))
                {
                    return 0;
                }

                /* Relax edge */
                if (distances[i] + distance <
                    distances[next])
                {
                    distances[next] =
                        distances[i] + distance;

                    previous[next] = i;

                    updated = 1;
                }
            }
        }

        /* Early stopping */
        if (!updated)
            break;
    }

    /* Check for negative-weight cycle */
    for (i = 0; i < graph->node_count; i++)
    {
        if (distances[i] == DBL_MAX)
            continue;

        for (j = 0;
             j < graph->nodes[i].edge_count;
             j++)
        {
            int next =
                graph->nodes[i].edges[j].destination;

            double distance =
                graph->nodes[i].edges[j].distance;

            if (distances[i] + distance <
                distances[next])
            {
                return -1;
            }
        }
    }

    return 1;
}