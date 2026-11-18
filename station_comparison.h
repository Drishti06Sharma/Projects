
#ifndef STATION_COMPARISON_H
#define STATION_COMPARISON_H

#include "graph.h"

void compare_station_strategies(const Graph *graph);

int run_greedy_strategy(
    const Graph *graph,
    const double demand[MAX_NODES],
    int selected[MAX_NODES],
    int strategy,
    int max_stations);

#endif
