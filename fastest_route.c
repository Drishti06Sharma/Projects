
#include <float.h>
#include <stddef.h>
#include "fastest_route.h"

int fastest_route(const Graph *graph, int source, int destination,
                  double times[MAX_NODES],
                  int previous[MAX_NODES])
{
    int visited[MAX_NODES] = {0};
    int i, count;

    if (graph == NULL ||
        source < 0 || source >= graph->node_count ||
        destination < 0 || destination >= graph->node_count)
    {
        return 0;
    }

    for (i = 0; i < graph->node_count; i++)
    {
        times[i] = DBL_MAX;
        previous[i] = -1;
    }

    times[source] = 0.0;

    for (count = 0; count < graph->node_count; count++)
    {
        int current = -1;
        double minimum = DBL_MAX;

        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] && times[i] < minimum)
            {
                minimum = times[i];
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
            double travel_time = graph->nodes[current].edges[i].time;

            if (!visited[next] &&
                times[current] + travel_time < times[next])
            {
                times[next] = times[current] + travel_time;
                previous[next] = current;
            }
        }
    }

    return 1;
}
