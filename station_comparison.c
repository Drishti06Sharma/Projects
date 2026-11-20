#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <ctype.h>
#include "station_comparison.h"

/* Simulation configuration */
#define DEFAULT_STATION_COST 400.0
#define DEFAULT_CHARGERS_PER_STATION 4
#define DEFAULT_MAX_CHARGERS 20
#define DEFAULT_CUSTOM_DEMAND 60.0

#define PREDEFINED_LOCATION_COUNT 10

/* Synthetic demand scores for predefined locations */
static const double predefined_demand[] =
{
    90.0,  /* Clock Tower */
    70.0,  /* ISBT Dehradun */
    65.0,  /* Railway Station */
    55.0,  /* Forest Research Institute */
    95.0,  /* Pacific Mall */
    75.0,  /* Prem Nagar */
    80.0,  /* DIT University */
    85.0,  /* Max Hospital */
    88.0,  /* Survey Chowk */
    92.0   /* Sahastradhara Road */
};

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

    double total_cost =
        station_count * station_cost;

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
            printf("Station: %s\n",
                   graph->nodes[i].name);

            printf("Road connections: %d\n",
                   graph->nodes[i].edge_count);

            printf("Simulated demand score: %.2f\n",
                   demand[i]);

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
                {
                    best = i;
                }
            }
            else
            {
                /* Strategy B: highest simulated demand */
                if (demand[i] > demand[best] ||
                    (demand[i] == demand[best] &&
                     graph->nodes[i].edge_count >
                     graph->nodes[best].edge_count))
                {
                    best = i;
                }
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

    const double station_cost =
        DEFAULT_STATION_COST;

    const int chargers_per_station =
        DEFAULT_CHARGERS_PER_STATION;

    const int maximum_chargers =
        DEFAULT_MAX_CHARGERS;

    int max_stations;
    int available_locations = 0;

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

    printf("\n===== SIMULATION CONFIGURATION =====\n");
    printf("Station installation cost: %.2f\n", station_cost);
    printf("Chargers per station: %d\n", chargers_per_station);
    printf("Maximum available chargers: %d\n", maximum_chargers);

    printf("\n===== LOCATION PRIORITY SCORES =====\n");

    /* Assign predefined synthetic demand scores */
    for (i = 0; i < graph->node_count; i++)
    {
        if (i < PREDEFINED_LOCATION_COUNT)
        {
            demand[i] = predefined_demand[i];
        }
        else
        {
            /* Default score for custom locations */
            demand[i] = DEFAULT_CUSTOM_DEMAND;
        }

        printf("%-25s | Connectivity: %d | Demand score: %.2f\n",
               graph->nodes[i].name,
               graph->nodes[i].edge_count,
               demand[i]);

        if (!graph->nodes[i].has_station)
            available_locations++;
    }

    if (available_locations == 0)
    {
        printf("\nAll locations already have charging stations.\n");
        return;
    }

    /* Only user input required */
    if (!read_positive_double(
            "\nEnter total installation budget: ", &budget))
    {
        printf("Invalid budget.\n");
        return;
    }

    /* Calculate station limit using budget and charger capacity */
    max_stations = (int)(budget / station_cost);

    int capacity_limit =
        maximum_chargers / chargers_per_station;

    if (max_stations > capacity_limit)
        max_stations = capacity_limit;

    if (max_stations > available_locations)
        max_stations = available_locations;

    printf("\n===== CONSTRAINTS =====\n");
    printf("Budget: %.2f\n", budget);
    printf("Cost per station: %.2f\n", station_cost);
    printf("Maximum stations allowed: %d\n", max_stations);

    /* Run both strategies */
    count_a = run_greedy_strategy(
        graph, demand, selected_a, 1, max_stations);

    count_b = run_greedy_strategy(
        graph, demand, selected_b, 2, max_stations);

    /* Display results */
    display_strategy_result(
        graph,
        selected_a,
        demand,
        count_a,
        chargers_per_station,
        station_cost,
        "STRATEGY A: CONNECTIVITY-BASED GREEDY");

    display_strategy_result(
        graph,
        selected_b,
        demand,
        count_b,
        chargers_per_station,
        station_cost,
        "STRATEGY B: DEMAND-BASED GREEDY");

    /* Compare both strategies */
    printf("\n===== COMPARISON SUMMARY =====\n");

    printf("Connectivity-based stations: %d\n", count_a);
    printf("Demand-based stations: %d\n", count_b);

    printf("\nNote: Demand scores are synthetic simulation values.\n");
    printf("Installation costs are hypothetical.\n");
    printf("Existing installed stations are excluded.\n");
    printf("This comparison does not modify the original graph.\n");
}