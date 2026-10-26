#include <stdio.h>
#include <float.h>
#include "bellman_ford.h"

int bellman_ford(const Graph *graph, int source,
                 double distances[MAX_NODES],
                 int previous[MAX_NODES])
{
    int i;

    if (graph == NULL || source < 0 ||
        source >= graph->node_count) {
        return 0;
    }

    for (i = 0; i < graph->node_count; i++) {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    distances[source] = 0.0;

    /* Bellman-Ford relaxation loop will go here */

    return 1;
}