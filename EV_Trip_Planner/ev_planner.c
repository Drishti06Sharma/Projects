
#include <stdio.h>
#include <float.h>
#include "ev_planner.h"
#include "input_validation.h"
#include "dijkstra.h"
#include "fastest_route.h"
#include "bellman_ford.h"


void show_menu(void)
{
    printf("\n===== EV Trip Planner =====\n");
    printf("1. Add Location\n");
    printf("2. Add Road\n");
    printf("3. Display Locations\n");
    printf("4. Find Shortest Distance Route (Dijkstra)\n");
    printf("5. Find Fastest Route (Dijkstra)\n");
    printf("6. Find Shortest Distance Route (Bellman-Ford)\n");
    printf("7. Install Charging Station\n");
    printf("8. Display Charging Stations\n");
    printf("9. Exit\n");
    printf("10. Automatic Charging Station Placement (Greedy)\n");
}
 

void add_location_menu(Graph *graph)
{
    char name[MAX_NAME];

    if (!read_string("Enter location name: ", name, MAX_NAME))
    {
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
    double distance, time;

    if (graph->node_count < 2)
    {
        printf("Add at least two locations first.\n");
        return;
    }

    display_locations(graph);

    if (!read_integer("Enter source location ID: ", &source) ||
        !read_integer("Enter destination location ID: ", &destination) ||
        !read_double("Enter road distance in km: ", &distance) ||
        !read_double("Enter travel time in hours: ", &time))
    {
        printf("Invalid input. Please try again.\n");
        return;
    }

    if (add_road(graph, source, destination, distance, time))
        printf("Road added successfully!\n");
    else
        printf("Could not add road. Check IDs, distance and time.\n");
}

static void display_route(const Graph *graph, int destination,
                          const int previous[MAX_NODES],
                          double total, const char *label,
                          const char *unit)
{
    int path[MAX_NODES];
    int count = 0;
    int current = destination;

    while (current != -1 && count < MAX_NODES)
    {
        path[count++] = current;
        current = previous[current];
    }

    printf("\n%s: ", label);

    for (int i = count - 1; i >= 0; i--)
    {
        printf("%s", graph->nodes[path[i]].name);

        if (i > 0)
            printf(" -> ");
    }

    printf("\nTotal %s: %.2f %s\n", label, total, unit);
}

static int get_route_locations(const Graph *graph,
                               int *source, int *destination)
{
    if (graph->node_count < 2)
    {
        printf("Add at least two locations first.\n");
        return 0;
    }

    display_locations(graph);

    if (!read_integer("Enter source location ID: ", source) ||
        !read_integer("Enter destination location ID: ", destination))
    {
        printf("Invalid input.\n");
        return 0;
    }

    if (*source < 0 || *source >= graph->node_count ||
        *destination < 0 || *destination >= graph->node_count)
    {
        printf("Invalid location ID.\n");
        return 0;
    }

    return 1;
}

void find_shortest_route(Graph *graph)
{
    int source, destination;
    double distances[MAX_NODES];
    int previous[MAX_NODES];

    if (!get_route_locations(graph, &source, &destination))
        return;

    if (!dijkstra(graph, source, destination, distances, previous))
    {
        printf("Unable to calculate route.\n");
        return;
    }

    if (distances[destination] == DBL_MAX)
    {
        printf("No route exists between these locations.\n");
        return;
    }

    display_route(graph, destination, previous,
                  distances[destination], "Shortest route (Dijkstra)", "km");
}

void find_fastest_route(Graph *graph)
{
    int source, destination;
    double times[MAX_NODES];
    int previous[MAX_NODES];

    if (!get_route_locations(graph, &source, &destination))
        return;

    if (!fastest_route(graph, source, destination, times, previous))
    {
        printf("Unable to calculate fastest route.\n");
        return;
    }

    if (times[destination] == DBL_MAX)
    {
        printf("No route exists between these locations.\n");
        return;
    }

    display_route(graph, destination, previous,
                  times[destination], "Fastest route (Dijkstra)", "hours");
}

void find_bellman_ford_route(Graph *graph)
{
    int source, destination;
    double distances[MAX_NODES];
    int previous[MAX_NODES];
    int result;

    if (!get_route_locations(graph, &source, &destination))
        return;

    result = bellman_ford(graph, source, distances, previous);

    if (result == 0)
    {
        printf("Unable to calculate route.\n");
        return;
    }

    if (result == -1)
    {
        printf("Negative-weight cycle detected. Shortest route is undefined.\n");
        return;
    }

    if (distances[destination] == DBL_MAX)
    {
        printf("No route exists between these locations.\n");
        return;
    }

    display_route(graph, destination, previous,
                  distances[destination], "Shortest route (Bellman-Ford)", "km");
}

void show_all_locations(Graph *graph)
{
    display_locations(graph);
}


void install_charging_station_menu(Graph *graph)
{
    int location_id;
    int charger_count;
    double cost;

    if (graph->node_count == 0)
    {
        printf("Please add locations first.\n");
        return;
    }

    display_locations(graph);

    if (!read_integer("Enter location ID for station: ", &location_id))
    {
        printf("Invalid location ID.\n");
        return;
    }

    if (!read_integer("Enter number of chargers: ", &charger_count))
    {
        printf("Invalid charger count.\n");
        return;
    }

    if (!read_double("Enter installation cost: ", &cost))
    {
        printf("Invalid installation cost.\n");
        return;
    }

    if (install_charging_station(
            graph, location_id, charger_count, cost))
    {
        printf("Charging station installed successfully!\n");
    }
    else
    {
        printf("Could not install station. Check the ID and values.\n");
    }
}

void show_charging_stations_menu(Graph *graph)
{
    display_charging_stations(graph);
}

void automatic_station_placement_menu(Graph *graph)
{
    int chargers;
    int placed = 0;
    int i,best;
    double budget, cost, spent = 0.0;

    if (graph->node_count == 0) {
        printf("Please add locations first.\n");
        return;
    }

    if (!read_double("Enter total installation budget: ", &budget) ||
        budget <= 0) {
        printf("Invalid budget.\n");
        return;
    }

    if (!read_double("Enter cost per station: ", &cost) ||
        cost <= 0) {
        printf("Invalid station cost.\n");
        return;
    }

    if (!read_integer("Enter chargers per station: ", &chargers) ||
        chargers <= 0) {
        printf("Invalid charger count.\n");
        return;
    }

    while (spent + cost <= budget) {
        best = -1;

        for (i = 0; i < graph->node_count; i++) {
            if (graph->nodes[i].has_station)
                continue;

            if (best == -1 ||
                graph->nodes[i].edge_count >
                graph->nodes[best].edge_count) {
                best = i;
            }
        }

        if (best == -1)
            break;

        if (!install_charging_station(
                graph, best, chargers, cost)) {
            printf("Could not install station at %s.\n",
                   graph->nodes[best].name);
            break;
        }

        spent += cost;
        placed++;

        printf("Station placed at %s "
               "(road connections: %d)\n",
               graph->nodes[best].name,
               graph->nodes[best].edge_count);
    }

    printf("\n--- Automatic Placement Summary ---\n");
    printf("Stations placed: %d\n", placed);
    printf("Total cost: %.2f\n", spent);
    printf("Remaining budget: %.2f\n", budget - spent);

    if (placed == 0)
        printf("No station could be placed within the budget.\n");
}