
#include <stdio.h>
#include "ev_planner.h"
#include "input_validation.h"

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
    char name[MAX_NAME];

    if (read_string("Enter location name: ", name, MAX_NAME)) {
        int result = add_location(graph, name);

        if (result >= 0)
            printf("Location added successfully!\n");
        else
            printf("Could not add location.\n");
    } else {
        printf("Invalid location name.\n");
    }
}

void add_road_menu(Graph *graph)
{
    (void)graph;
    printf("Add Road functionality is pending.\n");
}

void find_shortest_route(Graph *graph)
{
    (void)graph;
    printf("Shortest Route functionality is pending.\n");
}

void show_all_locations(Graph *graph)
{
    display_locations(graph);
}
