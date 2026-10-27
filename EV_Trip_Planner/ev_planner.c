
#include <stdio.h>
#include "ev_planner.h"
#include "input_validation.h"
#include "dijkstra.h"

void show_menu(void)
{
    printf("\n===== EV Trip Planner =====\n");
    printf("1. Add Location\n");
    printf("2. Add Road\n");
    printf("3. Display Locations\n");
    printf("4. Find Shortest Route\n");
    printf("5. Exit\n");
}

void add_location_menu(Graph *graph)
{
    char name[MAX_NAME];

    if (!read_string("Enter location name: ", name, MAX_NAME)) {
        printf("Invalid location name.\n");
        return;
    }

    if (add_location(graph, name) >= 0)
        printf("Location added successfully!\n");
    else
        printf("Could not add location.\n");
}

void add_road_menu(Graph *graph)
{
    int source, destination;
    double distance;

    if (graph->node_count < 2) {
        printf("Add at least two locations first.\n");
        return;
    }

    display_locations(graph);

    if (!read_integer("Enter source location ID: ", &source) ||
        !read_integer("Enter destination location ID: ", &destination) ||
        !read_double("Enter road distance in km: ", &distance)) {
        printf("Invalid input. Please try again.\n");
        return;
    }

    if (add_road(graph, source, destination, distance))
        printf("Road added successfully!\n");
    else
        printf("Could not add road. Check IDs and distance.\n");
}

void find_shortest_route(Graph *graph)
{
    int source, destination;
    double distances[MAX_NODES];
    int previous[MAX_NODES];

    if (graph->node_count < 2) {
        printf("Add at least two locations first.\n");
        return;
    }

    display_locations(graph);

    if (!read_integer("Enter source location ID: ", &source) ||
        !read_integer("Enter destination location ID: ", &destination)) {
        printf("Invalid input.\n");
        return;
    }

    if (source < 0 || source >= graph->node_count ||
        destination < 0 || destination >= graph->node_count) {
        printf("Invalid location ID.\n");
        return;
    }

    if (dijkstra(graph, source, destination, distances, previous)) {
        printf("Dijkstra function called.\n");
        printf("Shortest-route calculation is not implemented yet.\n");
    } else {
        printf("Unable to calculate route.\n");
    }
}

void show_all_locations(Graph *graph)
{
    display_locations(graph);
}
