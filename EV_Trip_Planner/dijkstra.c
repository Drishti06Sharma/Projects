#include <stdio.h>
#include <float.h>
#include "dijkstra.h"

int dijkstra(const Graph *graph, int source, int destination,
             double distances[MAX_NODES],
             int previous[MAX_NODES])
{
    int visited[MAX_NODES] = {0};
    int i;

    if (graph == NULL || source < 0 ||
        source >= graph->node_count ||
        destination < 0 ||
        destination >= graph->node_count) {
        return 0;
    }

    for (i = 0; i < graph->node_count; i++) {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    distances[source] = 0.0;

    /* Dijkstra's main loop will go here */

    return 1;
}