#include <stdio.h>
#include "ev_planner.h"

void show_menu(void)
{
    printf("\n===== EV Trip Planner =====\n");
    printf("1. Add Location\n");
    printf("2. Add Road\n");
    printf("3. Display Locations\n");
    printf("4. Find Shortest Route\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void add_location_menu(Graph *graph)
{
    printf("\nAdd Location selected.\n");
    printf("Location entry will be handled here.\n");
}

void add_road_menu(Graph *graph)
{
    printf("\nAdd Road selected.\n");
    printf("Road entry will be handled here.\n");
}

void find_shortest_route(Graph *graph)
{
    printf("\nShortest Route selected.\n");
    printf("Route calculation will be handled here.\n");
}

void show_all_locations(Graph *graph)
{
    display_locations(graph);
}