
#include <stdio.h>
#include "graph.h"
#include "ev_planner.h"
#include "input_validation.h"
#include "advanced_algorithms.h"

int main(void)
{
    Graph graph;
    int choice;

    initialize_graph(&graph);

    printf("Welcome to EV Trip Planner!\n");

    while (1)
    {
        show_menu();

        if (!read_integer("Enter your choice: ", &choice))
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                add_location_menu(&graph);
                break;

            case 2:
                add_road_menu(&graph);
                break;

            case 3:
                show_all_locations(&graph);
                break;

            case 4:
                find_shortest_route(&graph);
                break;

            case 5:
                find_fastest_route(&graph);
                break;

            case 6:
                find_bellman_ford_route(&graph);
                break;

            case 7:
                install_charging_station_menu(&graph);
                break;

            case 8:
                show_charging_stations_menu(&graph);
                break;

            case 9:
                printf("Exiting EV Trip Planner. Goodbye!\n");
                return 0;

            case 10:
                automatic_station_placement_menu(&graph);
                break;

            case 11:
                advanced_algorithms_menu();
                break;

            default:
                printf("Invalid choice. Please enter 1 to 11.\n");
        }
    }

    return 0;
}
