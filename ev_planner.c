#include <stdio.h>
#include <float.h>

#include "ev_planner.h"
#include "input_validation.h"
#include "dijkstra.h"
#include "fastest_route.h"
#include "bellman_ford.h"
#include "advanced_algorithms.h"

/* ---------- MENU ---------- */

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
    printf("9. Automatic Charging Station Placement (Greedy)\n");
    printf("10. Quick Sort and Binary Search\n");
    printf("11. Compare Charging Station Strategies\n");
    printf("12. Large Data Stress Test\n");
    printf("13. 0/1 Knapsack Station Optimization (DP)\n");
    printf("14. MST Comparison (Prim vs Kruskal)\n");
    printf("15. Exit\n");
}


/* ---------- ADD LOCATION ---------- */

void add_location_menu(Graph *graph)
{
    char name[MAX_NAME];

    if (graph == NULL)
    {
        printf("Invalid graph.\n");
        return;
    }

    if (!read_string("Enter location name: ",
                     name, MAX_NAME))
    {
        printf("Invalid location name.\n");
        return;
    }

    if (add_location(graph, name) >= 0)
    {
        printf("Location added successfully!\n");
    }
    else
    {
        printf("Could not add location.\n");
    }
}


/* ---------- ADD ROAD ---------- */

void add_road_menu(Graph *graph)
{
    int source, destination;
    double distance, time;

    if (graph == NULL)
    {
        printf("Invalid graph.\n");
        return;
    }

    if (graph->node_count < 2)
    {
        printf("Add at least two locations first.\n");
        return;
    }

    display_locations(graph);

    if (!read_integer("Enter source location ID: ",
                      &source) ||
        !read_integer("Enter destination location ID: ",
                      &destination) ||
        !read_double("Enter road distance in km: ",
                     &distance) ||
        !read_double("Enter travel time in hours: ",
                     &time))
    {
        printf("Invalid input. Please try again.\n");
        return;
    }

    if (add_road(graph, source, destination,
                 distance, time))
    {
        printf("Road added successfully!\n");
    }
    else
    {
        printf("Could not add road. "
               "Check IDs, distance and time.\n");
    }
}


/* ---------- DISPLAY ROUTE ---------- */

static void display_route(const Graph *graph,
                         int source,
                         int destination,
                         const int previous[MAX_NODES],
                         double total,
                         const char *label,
                         const char *unit)
{
    int path[MAX_NODES];
    int count = 0;
    int current = destination;

    if (graph == NULL || previous == NULL ||
        source < 0 || source >= graph->node_count ||
        destination < 0 ||
        destination >= graph->node_count)
    {
        printf("Invalid route data.\n");
        return;
    }

    /* Reconstruct path from destination to source */
    while (current != -1 && count < MAX_NODES)
    {
        if (current < 0 ||
            current >= graph->node_count)
        {
            printf("Invalid path detected.\n");
            return;
        }

        path[count++] = current;

        if (current == source)
            break;

        current = previous[current];
    }

    /* Verify that the path reaches source */
    if (count == 0 || path[count - 1] != source)
    {
        printf("Unable to reconstruct route.\n");
        return;
    }

    printf("\n%s: ", label);

    /* Print path in forward order */
    for (int i = count - 1; i >= 0; i--)
    {
        printf("%s", graph->nodes[path[i]].name);

        if (i > 0)
        {
            printf(" -> ");
        }
    }

    printf("\nTotal %s: %.2f %s\n",
           label, total, unit);
}


/* ---------- ROUTE INPUT ---------- */

static int get_route_locations(const Graph *graph,
                              int *source,
                              int *destination)
{
    if (graph == NULL ||
        source == NULL ||
        destination == NULL)
    {
        printf("Invalid input.\n");
        return 0;
    }

    if (graph->node_count < 2)
    {
        printf("Add at least two locations first.\n");
        return 0;
    }

    display_locations(graph);

    if (!read_integer("Enter source location ID: ",
                     source) ||
        !read_integer("Enter destination location ID: ",
                     destination))
    {
        printf("Invalid input.\n");
        return 0;
    }

    if (*source < 0 ||
        *source >= graph->node_count ||
        *destination < 0 ||
        *destination >= graph->node_count)
    {
        printf("Invalid location ID.\n");
        return 0;
    }

    return 1;
}


/* ---------- DIJKSTRA: SHORTEST DISTANCE ---------- */

void find_shortest_route(Graph *graph)
{
    int source, destination;

    double distances[MAX_NODES];
    int previous[MAX_NODES];

    if (!get_route_locations(graph,
                             &source,
                             &destination))
    {
        return;
    }

    if (!dijkstra(graph, source, destination,
                 distances, previous))
    {
        printf("Unable to calculate route.\n");
        return;
    }

    if (distances[destination] == DBL_MAX)
    {
        printf("No route exists between "
               "these locations.\n");
        return;
    }

    display_route(graph,
                  source,
                  destination,
                  previous,
                  distances[destination],
                  "Shortest route (Dijkstra)",
                  "km");
}


/* ---------- DIJKSTRA: FASTEST ROUTE ---------- */

void find_fastest_route(Graph *graph)
{
    int source, destination;

    double times[MAX_NODES];
    int previous[MAX_NODES];

    if (!get_route_locations(graph,
                             &source,
                             &destination))
    {
        return;
    }

    if (!fastest_route(graph, source, destination,
                       times, previous))
    {
        printf("Unable to calculate fastest route.\n");
        return;
    }

    if (times[destination] == DBL_MAX)
    {
        printf("No route exists between "
               "these locations.\n");
        return;
    }

    display_route(graph,
                  source,
                  destination,
                  previous,
                  times[destination],
                  "Fastest route (Dijkstra)",
                  "hours");
}


/* ---------- BELLMAN-FORD ---------- */

void find_bellman_ford_route(Graph *graph)
{
    int source, destination;

    double distances[MAX_NODES];
    int previous[MAX_NODES];

    int result;

    if (!get_route_locations(graph,
                             &source,
                             &destination))
    {
        return;
    }

    result = bellman_ford(graph, source,
                         distances, previous);

    if (result == 0)
    {
        printf("Unable to calculate route.\n");
        return;
    }

    if (result == -1)
    {
        printf("Negative-weight cycle detected. "
               "Shortest route is undefined.\n");
        return;
    }

    if (distances[destination] == DBL_MAX)
    {
        printf("No route exists between "
               "these locations.\n");
        return;
    }

    display_route(graph,
                  source,
                  destination,
                  previous,
                  distances[destination],
                  "Shortest route (Bellman-Ford)",
                  "km");
}


/* ---------- DISPLAY LOCATIONS ---------- */

void show_all_locations(Graph *graph)
{
    if (graph == NULL)
    {
        printf("Invalid graph.\n");
        return;
    }

    display_locations(graph);
}


/* ---------- MANUAL STATION INSTALLATION ---------- */

void install_charging_station_menu(Graph *graph)
{
    int location_id;
    int charger_count;

    double cost;

    if (graph == NULL ||
        graph->node_count == 0)
    {
        printf("Please add locations first.\n");
        return;
    }

    display_locations(graph);

    if (!read_integer(
            "Enter location ID for station: ",
            &location_id))
    {
        printf("Invalid location ID.\n");
        return;
    }

    if (!read_integer(
            "Enter number of chargers: ",
            &charger_count))
    {
        printf("Invalid charger count.\n");
        return;
    }

    if (!read_double(
            "Enter installation cost: ",
            &cost))
    {
        printf("Invalid installation cost.\n");
        return;
    }

    if (install_charging_station(
            graph,
            location_id,
            charger_count,
            cost))
    {
        printf("Charging station installed "
               "successfully!\n");
    }
    else
    {
        printf("Could not install station. "
               "Check the ID and values.\n");
    }
}


/* ---------- DISPLAY STATIONS ---------- */

void show_charging_stations_menu(Graph *graph)
{
    if (graph == NULL)
    {
        printf("Invalid graph.\n");
        return;
    }

    display_charging_stations(graph);
}


/* ---------- AUTOMATIC STATION PLACEMENT: GREEDY ---------- */

void automatic_station_placement_menu(Graph *graph)
{
    const double station_cost = 400.0;
    const int chargers_per_station = 4;

    int placed = 0;
    int i;
    int best;

    double budget;
    double spent = 0.0;

    if (graph == NULL ||
        graph->node_count <= 0)
    {
        printf("Please add locations first.\n");
        return;
    }

    if (graph->node_count > MAX_NODES)
    {
        printf("Invalid graph size.\n");
        return;
    }

    /* Validate connectivity data */
    for (i = 0; i < graph->node_count; i++)
    {
        if (graph->nodes[i].edge_count < 0 ||
            graph->nodes[i].edge_count > MAX_NODES)
        {
            printf("Invalid graph connection data.\n");
            return;
        }
    }

    printf("\n===== AUTOMATIC STATION PLACEMENT =====\n");

    printf("Station cost: %.2f\n",
           station_cost);

    printf("Chargers per station: %d\n",
           chargers_per_station);

    if (!read_double(
            "Enter total installation budget: ",
            &budget) ||
        budget <= 0)
    {
        printf("Invalid budget.\n");
        return;
    }

    /* Continue while budget permits */
    while (budget - spent >= station_cost)
    {
        best = -1;

        /* Find location with maximum connectivity */
        for (i = 0; i < graph->node_count; i++)
        {
            /* Skip existing stations */
            if (graph->nodes[i].has_station)
            {
                continue;
            }

            /* Select most connected location */
            if (best == -1 ||
                graph->nodes[i].edge_count >
                graph->nodes[best].edge_count)
            {
                best = i;
            }
        }

        /* No available location */
        if (best == -1)
        {
            break;
        }

        /* Install selected station */
        if (!install_charging_station(
                graph,
                best,
                chargers_per_station,
                station_cost))
        {
            printf("Could not install station "
                   "at %s.\n",
                   graph->nodes[best].name);

            break;
        }

        spent += station_cost;
        placed++;

        printf("Station placed at %-25s | "
               "Road connections: %d | Cost: %.2f\n",
               graph->nodes[best].name,
               graph->nodes[best].edge_count,
               station_cost);
    }

    /* Final summary */
    printf("\n===== AUTOMATIC PLACEMENT SUMMARY =====\n");

    printf("Stations placed: %d\n",
           placed);

    printf("Chargers installed: %d\n",
           placed * chargers_per_station);

    printf("Total cost: %.2f\n",
           spent);

    printf("Remaining budget: %.2f\n",
           budget - spent);

    if (placed == 0)
    {
        printf("No station could be placed within "
               "the budget or all locations already "
               "have stations.\n");
    }
}