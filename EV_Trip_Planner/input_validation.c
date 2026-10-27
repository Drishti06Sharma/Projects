
#include <stdio.h>
#include "graph.h"
#include "ev_planner.h"
#include "input_validation.h"

int main(void)
{
    Graph graph;
    int choice;

    initialize_graph(&graph);

    printf("Welcome to EV Trip Planner!\n");

    while (1) {
        show_menu();

        if (!read_integer("Enter your choice: ", &choice)) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
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
                printf("Exiting EV Trip Planner. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Select 1 to 5.\n");
        }
    }
}
