#include <stdio.h>
#include <stdlib.h>
#include "knapsack.h"
#include "input_validation.h"

#define MAX_BUDGET 100000

void knapsack_station_selection(Graph *graph)
{
    int i, w;
    int budget;
    int candidate_count = 0;

    int costs[MAX_NODES];
    int benefits[MAX_NODES];
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

    printf("\nAvailable Locations:\n");

    for (i = 0; i < graph->node_count; i++)
    {
        if (!graph->nodes[i].has_station)
        {
            printf("%d. %-25s | Road connections: %d\n",
                   i,
                   graph->nodes[i].name,
                   graph->nodes[i].edge_count);
        }
    }

    printf("\nEnter installation cost for each available location.\n");

    for (i = 0; i < graph->node_count; i++)
    {
        if (graph->nodes[i].has_station)
            continue;

        int cost;

        printf("\nLocation: %s\n",
               graph->nodes[i].name);

        if (!read_integer("Installation cost: ", &cost) ||
            cost <= 0)
        {
            printf("Invalid cost. Knapsack cancelled.\n");
            return;
        }

        costs[candidate_count] = cost;

        /* Benefit is based on road connectivity */
        benefits[candidate_count] =
            graph->nodes[i].edge_count;

        candidate_count++;
    }

    if (candidate_count == 0)
    {
        printf("All locations already have charging stations.\n");
        return;
    }

    /* Allocate DP table */
    int cols = budget + 1;

    int *dp = calloc(
        (size_t)(candidate_count + 1) * cols,
        sizeof(int)
    );

    if (dp == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    /* Dynamic Programming */
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

    /* Backtracking */
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
            /* Find corresponding graph location */
            int candidate_index = 0;
            int location_id = -1;
            int j;

            for (j = 0; j < graph->node_count; j++)
            {
                if (graph->nodes[j].has_station)
                    continue;

                if (candidate_index == i)
                {
                    location_id = j;
                    break;
                }

                candidate_index++;
            }

            if (location_id != -1)
            {
                printf("%s | Cost: %d | Benefit: %d\n",
                       graph->nodes[location_id].name,
                       costs[i],
                       benefits[i]);

                total_cost += costs[i];
                total_benefit += benefits[i];
                stations_selected++;
            }
        }
    }

    printf("\n===== SUMMARY =====\n");
    printf("Stations selected: %d\n", stations_selected);
    printf("Total installation cost: %d\n", total_cost);
    printf("Remaining budget: %d\n", budget - total_cost);
    printf("Total connectivity benefit: %d\n", total_benefit);

    free(dp);
}