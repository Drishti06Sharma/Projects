#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "graph.h"

/* ---------- LOAD DEHRADUN NETWORK ---------- */

void load_dehradun_network(Graph *graph)
{
    if (graph == NULL)
        return;

    graph->node_count = 0;

    /* Predefined Dehradun locations */
    add_location(graph, "Clock Tower");
    add_location(graph, "ISBT Dehradun");
    add_location(graph, "Railway Station");
    add_location(graph, "Forest Research Institute");
    add_location(graph, "Pacific Mall");
    add_location(graph, "Prem Nagar");
    add_location(graph, "DIT University");
    add_location(graph, "Max Hospital");
    add_location(graph, "Survey Chowk");
    add_location(graph, "Sahastradhara Road");

    /* Clock Tower connections */
    add_road(graph, 0, 1, 5.5, 0.20);
    add_road(graph, 0, 2, 2.0, 0.10);
    add_road(graph, 0, 3, 4.5, 0.18);
    add_road(graph, 0, 4, 6.5, 0.22);
    add_road(graph, 0, 8, 1.8, 0.10);

    /* ISBT connections */
    add_road(graph, 1, 2, 4.0, 0.15);
    add_road(graph, 1, 5, 8.5, 0.30);

    /* Railway Station connections */
    add_road(graph, 2, 8, 2.5, 0.12);

    /* FRI connections */
    add_road(graph, 3, 5, 6.0, 0.22);

    /* Pacific Mall connections */
    add_road(graph, 4, 7, 4.0, 0.15);
    add_road(graph, 4, 9, 6.5, 0.25);
    add_road(graph, 4, 6, 12.0, 0.35);

    /* Prem Nagar connections */
    add_road(graph, 5, 6, 18.0, 0.45);

    /* DIT University connections */
    add_road(graph, 6, 9, 14.0, 0.40);

    /* Max Hospital connections */
    add_road(graph, 7, 8, 4.0, 0.16);

    /* Sahastradhara Road connection */
    add_road(graph, 9, 8, 8.0, 0.25);
}

/* ---------- INITIALIZE GRAPH ---------- */

void initialize_graph(Graph *graph)
{
    if (graph == NULL)
        return;

    graph->node_count = 0;

    load_dehradun_network(graph);
}

/* ---------- ADD LOCATION ---------- */

int add_location(Graph *graph, const char *name)
{
    int i;
    int length;

    if (graph == NULL || name == NULL)
        return -1;

    if (graph->node_count < 0 ||
        graph->node_count >= MAX_NODES)
    {
        printf("Maximum locations reached!\n");
        return -1;
    }

    /* Remove leading spaces */
    while (isspace((unsigned char)*name))
    {
        name++;
    }

    if (*name == '\0')
    {
        printf("Invalid location name.\n");
        return -1;
    }

    /* Check name length */
    length = strlen(name);

    if (length >= MAX_NAME)
    {
        printf("Location name is too long.\n");
        return -1;
    }

    /* Remove trailing spaces */
    while (length > 0 &&
           isspace((unsigned char)name[length - 1]))
    {
        length--;
    }

    if (length == 0)
    {
        printf("Invalid location name.\n");
        return -1;
    }

    /* Check duplicate location names */
    for (i = 0; i < graph->node_count; i++)
    {
        if (strcmp(graph->nodes[i].name, name) == 0)
        {
            printf("Location already exists!\n");
            return -1;
        }
    }

    i = graph->node_count;

    strncpy(graph->nodes[i].name, name, length);
    graph->nodes[i].name[length] = '\0';

    graph->nodes[i].edge_count = 0;

    graph->nodes[i].has_station = 0;
    graph->nodes[i].charger_count = 0;
    graph->nodes[i].installation_cost = 0.0;

    graph->node_count++;

    return i;
}

/* ---------- ADD ROAD ---------- */

int add_road(Graph *graph,
             int source,
             int destination,
             double distance,
             double time)
{
    int i;

    if (graph == NULL)
        return 0;

    /* Validate location IDs */
    if (source < 0 || source >= graph->node_count ||
        destination < 0 || destination >= graph->node_count)
    {
        printf("Invalid location ID.\n");
        return 0;
    }

    /* Prevent self-loops */
    if (source == destination)
    {
        printf("A location cannot connect to itself.\n");
        return 0;
    }

    /* Validate road values */
    if (distance <= 0 || time <= 0)
    {
        printf("Distance and time must be positive.\n");
        return 0;
    }

    /* Check edge capacity */
    if (graph->nodes[source].edge_count >= MAX_NODES ||
        graph->nodes[destination].edge_count >= MAX_NODES)
    {
        printf("Maximum road connections reached.\n");
        return 0;
    }

    /* Check for duplicate road */
    for (i = 0; i < graph->nodes[source].edge_count; i++)
    {
        if (graph->nodes[source].edges[i].destination ==
            destination)
        {
            printf("Road already exists between these locations.\n");
            return 0;
        }
    }

    /* Add road from source to destination */
    i = graph->nodes[source].edge_count;

    graph->nodes[source].edges[i].destination = destination;
    graph->nodes[source].edges[i].distance = distance;
    graph->nodes[source].edges[i].time = time;

    graph->nodes[source].edge_count++;

    /* Add reverse road for undirected graph */
    i = graph->nodes[destination].edge_count;

    graph->nodes[destination].edges[i].destination = source;
    graph->nodes[destination].edges[i].distance = distance;
    graph->nodes[destination].edges[i].time = time;

    graph->nodes[destination].edge_count++;

    return 1;
}

/* ---------- INSTALL CHARGING STATION ---------- */

int install_charging_station(Graph *graph,
                             int location_id,
                             int charger_count,
                             double installation_cost)
{
    if (graph == NULL)
        return 0;

    /* Validate location */
    if (location_id < 0 ||
        location_id >= graph->node_count)
    {
        printf("Invalid location ID.\n");
        return 0;
    }

    /* Validate charger count and cost */
    if (charger_count <= 0 ||
        installation_cost <= 0)
    {
        printf("Charger count and installation cost "
               "must be positive.\n");
        return 0;
    }

    /* Prevent duplicate station installation */
    if (graph->nodes[location_id].has_station)
    {
        printf("A charging station already exists "
               "at this location.\n");
        return 0;
    }

    graph->nodes[location_id].has_station = 1;

    graph->nodes[location_id].charger_count =
        charger_count;

    graph->nodes[location_id].installation_cost =
        installation_cost;

    return 1;
}

/* ---------- DISPLAY LOCATIONS ---------- */

void display_locations(const Graph *graph)
{
    int i, j;

    if (graph == NULL || graph->node_count == 0)
    {
        printf("No locations available.\n");
        return;
    }

    printf("\n--- EV Road Network ---\n");

    for (i = 0; i < graph->node_count; i++)
    {
        printf("\nLocation %d: %s",
               i,
               graph->nodes[i].name);

        if (graph->nodes[i].has_station)
        {
            printf(" [Charging Station]");
        }

        printf("\n");

        for (j = 0;
             j < graph->nodes[i].edge_count;
             j++)
        {
            int next =
                graph->nodes[i].edges[j].destination;

            printf("  -> %s (%.2f km, %.2f hours)\n",
                   graph->nodes[next].name,
                   graph->nodes[i].edges[j].distance,
                   graph->nodes[i].edges[j].time);
        }
    }
}

/* ---------- DISPLAY CHARGING STATIONS ---------- */

void display_charging_stations(const Graph *graph)
{
    int i;
    int found = 0;

    if (graph == NULL)
    {
        printf("Invalid graph.\n");
        return;
    }

    printf("\n--- Charging Stations ---\n");

    for (i = 0; i < graph->node_count; i++)
    {
        if (graph->nodes[i].has_station)
        {
            printf("Location ID: %d\n", i);

            printf("Location: %s\n",
                   graph->nodes[i].name);

            printf("Number of chargers: %d\n",
                   graph->nodes[i].charger_count);

            printf("Installation cost: %.2f\n",
                   graph->nodes[i].installation_cost);

            printf("-------------------------\n");

            found = 1;
        }
    }

    if (!found)
    {
        printf("No charging stations installed yet.\n");
    }
}