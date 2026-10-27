
#ifndef FASTEST_ROUTE_H
#define FASTEST_ROUTE_H

#include "graph.h"

int fastest_route(const Graph *graph, int source, int destination,
                  double times[MAX_NODES],
                  int previous[MAX_NODES]);

#endif
