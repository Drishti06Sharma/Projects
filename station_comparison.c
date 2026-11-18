
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <ctype.h>
#include "station_comparison.h"

/* Read a valid positive number */
static int read_positive_double(const char *message, double *value)
{
    char input[100];
    char *end;
    double number;

    printf("%s", message);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    number = strtod(input, &end);

    while (isspace((unsigned char)*end))
        end++;

    if (end == input || *end != '\0' ||
        number <= 0.0 || number >= DBL_MAX)
        return 0;

    *value = number;
    return 1;
}

/* Read a valid positive integer */
static int read_positive_integer(const char *message, int *value)
{
    char input[100];
    char *end;
    long number;

    printf("%s", message);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    number = strtol(input, &end, 10);

    while (isspace((unsigned char)*end))
        end++;

    if (end == input || *end != '\0' ||
        number <= 0 || number > 1000000)
        return 0;

    *value = (int)number;
    return 1;
}

/* Display results of one placement strategy */
static void display_strategy_result(
    const Graph *graph,
    const int selected[MAX_NODES],
    const double demand[MAX_NODES],
    int station_count,
    int chargers_per_station,
    double station_cost,
    const char *strategy_name)
{
    int i;
    int total_chargers =
        station_count * chargers_per_station;

    double total_cost = station_count * station_cost;
    double captured_score = 0.0;

    printf("\n--- %s ---\n", strategy_name);

    if (station_count == 0)
    {
        printf("No stations could be installed.\n");
    }

    for (i = 0; i < graph->node_count; i++)
    {
        if (selected[i])
        {
            printf("Station: %s\n", graph->nodes[i].name);
            printf("Road connections: %d\n",
                   graph->nodes[i].edge_count);
            printf("Simulated demand score: %.2f\n", demand[i]);

            captured_score += demand[i];
        }
    }

    printf("Stations installed: %d\n", station_count);
    printf("Total chargers: %d\n", total_chargers);
    printf("Total installation cost: %.2f\n", total_cost);
    printf("Simulated priority score captured: %.2f\n",
           captured_score);
}

/* Run one greedy strategy without modifying the original graph */
int run_greedy_strategy(
    const Graph *graph,
    const double demand[MAX_NODES],
    int selected[MAX_NODES],
    int strategy,
    int max_stations)
{
    int placed = 0;
    int i;

    for (i = 0; i < graph->node_count; i++)
        selected[i] = 0;

    while (placed < max_stations)
    {
        int best = -1;

        for (i = 0; i < graph->node_count; i++)
        {
            if (selected[i] || graph->nodes[i].has_station)
                continue;

            if (best == -1)
            {
                best = i;
            }
            else if (strategy == 1)
            {
                /* Strategy A: highest road connectivity */
                if (graph->nodes[i].edge_count >
                    graph->nodes[best].edge_count)
                    best = i;
            }
            else
            {
                /* Strategy B: highest simulated demand */
                if (demand[i] > demand[best] ||
                    (demand[i] == demand[best] &&
                     graph->nodes[i].edge_count >
                     graph->nodes[best].edge_count))
                    best = i;
            }
        }

        if (best == -1)
            break;

        selected[best] = 1;
        placed++;
    }

    return placed;
}

/* Compare two charging station placement strategies */
void compare_station_strategies(const Graph *graph)
{
    double demand[MAX_NODES] = {0};
    int selected_a[MAX_NODES] = {0};
    int selected_b[MAX_NODES] = {0};

    double budget;
    double station_cost;

    int chargers_per_station;
    int maximum_chargers;
    int max_stations;

    int count_a;
    int count_b;
    int i;

    if (graph == NULL || graph->node_count == 0)
    {
        printf("No locations available.\n");
        return;
    }

    printf("\n====================================\n");
    printf(" CHARGING STATION STRATEGY COMPARISON\n");
    printf("====================================\n");

    printf("\nEnter simulated demand priority scores.\n");
    printf("Use values greater than zero for each location.\n");

    for (i = 0; i < graph->node_count; i++)
    {
        char message[120];

        snprintf(message, sizeof(message),
                 "Demand score for %s: ",
                 graph->nodes[i].name);

        if (!read_positive_double(message, &demand[i]))
        {
            printf("Invalid demand score. Operation cancelled.\n");
            return;
        }
    }

    if (!read_positive_double(
            "Enter total installation budget: ", &budget))
    {
        printf("Invalid budget.\n");
        return;
    }

    if (!read_positive_double(
            "Enter cost per station: ", &station_cost))
    {
        printf("Invalid station cost.\n");
        return;
    }

    if (!read_positive_integer(
            "Enter chargers per station: ", &chargers_per_station))
    {
        printf("Invalid charger count.\n");
        return;
    }

    if (!read_positive_integer(
            "Enter maximum available chargers: ", &maximum_chargers))
    {
        printf("Invalid maximum charger capacity.\n");
        return;
    }

    /* Apply both budget and capacity constraints */
    max_stations = (int)(budget / station_cost);

    if (max_stations > maximum_chargers / chargers_per_station)
        max_stations = maximum_chargers / chargers_per_station;

    if (max_stations > graph->node_count)
        max_stations = graph->node_count;

    if (max_stations < 0)
        max_stations = 0;

    printf("\nMaximum stations allowed: %d\n", max_stations);

    count_a = run_greedy_strategy(
        graph, demand, selected_a, 1, max_stations);

    count_b = run_greedy_strategy(
        graph, demand, selected_b, 2, max_stations);

    display_strategy_result(
        graph, selected_a, demand, count_a,
        chargers_per_station, station_cost,
        "STRATEGY A: CONNECTIVITY-BASED GREEDY");

    display_strategy_result(
        graph, selected_b, demand, count_b,
        chargers_per_station, station_cost,
        "STRATEGY B: DEMAND-BASED GREEDY");

    printf("\n--- COMPARISON SUMMARY ---\n");
    printf("Connectivity-based stations: %d\n", count_a);
    printf("Demand-based stations: %d\n", count_b);

    printf("\nNote: Demand scores are synthetic simulation values.\n");
    printf("Existing installed stations are excluded.\n");
    printf("This comparison does not modify the original graph.\n");
}
