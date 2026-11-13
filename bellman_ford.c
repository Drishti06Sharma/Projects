
#include <float.h>
#include <stddef.h>
#include "bellman_ford.h"

int bellman_ford(const Graph *graph, int source,
                 double distances[MAX_NODES],
                 int previous[MAX_NODES])
{
    int i, j, k;

    if (graph == NULL || distances == NULL || previous == NULL ||
        graph->node_count < 1 || graph->node_count > MAX_NODES ||
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

    for (k = 0; k < graph->node_count - 1; k++)
    {
        int updated = 0;

        for (i = 0; i < graph->node_count; i++)
        {
            if (distances[i] == DBL_MAX)
                continue;

            for (j = 0; j < graph->nodes[i].edge_count; j++)
            {
                int next = graph->nodes[i].edges[j].destination;
                double distance = graph->nodes[i].edges[j].distance;

                if (distances[i] + distance < distances[next])
                {
                    distances[next] = distances[i] + distance;
                    previous[next] = i;
                    updated = 1;
                }
            }
        }

        if (!updated)
            break;
    }

    for (i = 0; i < graph->node_count; i++)
    {
        if (distances[i] == DBL_MAX)
            continue;

        for (j = 0; j < graph->nodes[i].edge_count; j++)
        {
            int next = graph->nodes[i].edges[j].destination;
            double distance = graph->nodes[i].edges[j].distance;

            if (distances[i] + distance < distances[next])
                return -1;
        }
    }

    return 1;
}
