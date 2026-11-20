#include <stdio.h>
#include <stdlib.h>
#include "knapsack.h"
#include "input_validation.h"

#define MAX_BUDGET 100000
#define DEFAULT_CUSTOM_COST 400

/* Hypothetical installation costs for predefined locations */
static const int predefined_costs[] =
{
    500,  /* Clock Tower */
    300,  /* ISBT Dehradun */
    400,  /* Railway Station */
    200,  /* Forest Research Institute */
    600,  /* Pacific Mall */
    300,  /* Prem Nagar */
    400,  /* DIT University */
    200,  /* Max Hospital */
    300,  /* Survey Chowk */
    400   /* Sahastradhara Road */
};

#define PREDEFINED_LOCATION_COUNT 10

void knapsack_station_selection(Graph *graph)
{
    int i, w;
    int budget;
    int candidate_count = 0;

    int costs[MAX_NODES];
    int benefits[MAX_NODES];
    int location_ids[MAX_NODES];
    int selected[MAX_NODES] = {0};

    if (graph == NULL || graph->node_count == 0)
    {
        printf("No locations available.\n");
        return;
    }

    printf("\n===== 0/1 KNAPSACK STATION OPTIMIZATION =====\n");

    if (!read_integer("Enter total budget: ", &budget) ||
        budget <= 0 || budget > MAX_BUDGET)
    {
        printf("Invalid budget. Enter a value between 1 and %d.\n",
               MAX_BUDGET);
        return;
    }

    printf("\n===== AVAILABLE LOCATIONS =====\n");

    for (i = 0; i < graph->node_count; i++)
    {
        if (graph->nodes[i].has_station)
            continue;

        /* Store location ID */
        location_ids[candidate_count] = i;

        /* Assign predefined or default cost */
        if (i < PREDEFINED_LOCATION_COUNT)
        {
            costs[candidate_count] = predefined_costs[i];
        }
        else
        {
            costs[candidate_count] = DEFAULT_CUSTOM_COST;
        }

        /* Connectivity is used as benefit */
        benefits[candidate_count] =
            graph->nodes[i].edge_count;

        printf("%d. %-25s | Connections: %d | Cost: %d\n",
               i,
               graph->nodes[i].name,
               benefits[candidate_count],
               costs[candidate_count]);

        candidate_count++;
    }

    if (candidate_count == 0)
    {
        printf("All locations already have charging stations.\n");
        return;
    }

    /* Allocate Dynamic Programming table */
    int cols = budget + 1;

    size_t total_cells =
        (size_t)(candidate_count + 1) * (size_t)cols;

    int *dp = calloc(total_cells, sizeof(int));

    if (dp == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    /* 0/1 Knapsack Dynamic Programming */
    for (i = 1; i <= candidate_count; i++)
    {
        for (w = 0; w <= budget; w++)
        {
            int exclude =
                dp[(i - 1) * cols + w];

            int include = 0;

            if (costs[i - 1] <= w)
            {
                include =
                    benefits[i - 1] +
                    dp[(i - 1) * cols +
                       (w - costs[i - 1])];
            }

            dp[i * cols + w] =
                (include > exclude) ? include : exclude;
        }
    }

    printf("\nMaximum Connectivity Benefit: %d\n",
           dp[candidate_count * cols + budget]);

    /* Backtracking to identify selected locations */
    w = budget;

    for (i = candidate_count; i > 0; i--)
    {
        if (dp[i * cols + w] !=
            dp[(i - 1) * cols + w])
        {
            selected[i - 1] = 1;
            w -= costs[i - 1];
        }
    }

    printf("\n===== SELECTED STATIONS =====\n");

    int total_cost = 0;
    int total_benefit = 0;
    int stations_selected = 0;

    for (i = 0; i < candidate_count; i++)
    {
        if (selected[i])
        {
            int location_id = location_ids[i];

            printf("%-25s | Cost: %d | Benefit: %d\n",
                   graph->nodes[location_id].name,
                   costs[i],
                   benefits[i]);

            total_cost += costs[i];
            total_benefit += benefits[i];
            stations_selected++;
        }
    }

    printf("\n===== SUMMARY =====\n");
    printf("Stations selected: %d\n", stations_selected);
    printf("Total installation cost: %d\n", total_cost);
    printf("Remaining budget: %d\n", budget - total_cost);
    printf("Total connectivity benefit: %d\n", total_benefit);

    free(dp);
}