
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

int dijkstra(const Graph *graph, int source, int destination,
             double distances[MAX_NODES],
             int previous[MAX_NODES]);

#endif
