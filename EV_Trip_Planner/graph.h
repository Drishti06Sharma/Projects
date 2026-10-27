
#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 50
#define MAX_NAME 50

typedef struct
{
    int destination;
    double distance;
    double time;
} Edge;

typedef struct
{
    char name[MAX_NAME];
    int edge_count;
    Edge edges[MAX_NODES];

    int has_station;
    int charger_count;
    double installation_cost;
} Node;

typedef struct
{
    int node_count;
    Node nodes[MAX_NODES];
} Graph;

void initialize_graph(Graph *graph);

int add_location(Graph *graph, const char *name);

int add_road(Graph *graph, int source, int destination,
             double distance, double time);

void display_locations(const Graph *graph);

int install_charging_station(Graph *graph, int location_id,
                             int charger_count,
                             double installation_cost);

void display_charging_stations(const Graph *graph);

#endif
