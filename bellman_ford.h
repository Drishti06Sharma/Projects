
#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "graph.h"

int bellman_ford(const Graph *graph, int source,
                 double distances[MAX_NODES],
                 int previous[MAX_NODES]);

#endif
