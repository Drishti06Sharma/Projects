
#ifndef EV_PLANNER_H
#define EV_PLANNER_H

#include "graph.h"

void show_menu(void);
void add_location_menu(Graph *graph);
void add_road_menu(Graph *graph);
void find_shortest_route(Graph *graph);
void find_fastest_route(Graph *graph);
void find_bellman_ford_route(Graph *graph);
void show_all_locations(Graph *graph);
void install_charging_station_menu(Graph *graph);
void show_charging_stations_menu(Graph *graph);
#endif
