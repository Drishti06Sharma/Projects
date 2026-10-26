
#include <stdio.h>
#include "graph.h"

int main(void) {
    Graph graph;

    initialize_graph(&graph);

    int a = add_location(&graph, "Campus");
    int b = add_location(&graph, "Market");
    int c = add_location(&graph, "Hospital");
    int d = add_location(&graph, "Mall");

    if (a == -1 || b == -1 || c == -1 || d == -1) {
        return 1;
    }

    add_road(&graph, a, b, 3.0);
    add_road(&graph, a, c, 5.0);
    add_road(&graph, b, d, 2.0);
    add_road(&graph, c, d, 4.0);

    display_locations(&graph);

    return 0;
}
