#include <float.h>
#include <stddef.h>
#include <math.h>

#include "dijkstra.h"

/* Dijkstra: ek source se ek destination tak shortest path */

int dijkstra(const Graph *graph,
             int source,
             int destination,
             double distances[MAX_NODES],
             int previous[MAX_NODES])
{
    // Track karenge ki kaunsa node already process ho chuka hai
    int visited[MAX_NODES] = {0};
    int i, count;

    // Check kar rahe hain ki required pointers NULL toh nahi hain
    if (graph == NULL ||
        distances == NULL ||
        previous == NULL)
    {
        return 0; // Invalid input
    }

    // Check kar rahe hain ki graph mein valid number of nodes hain
    if (graph->node_count <= 0 ||
        graph->node_count > MAX_NODES)
    {
        return 0;
    }

    // Check kar rahe hain ki source and destination valid node indices hain
    if (source < 0 || source >= graph->node_count ||
        destination < 0 || destination >= graph->node_count)
    {
        return 0;
    }

    // Sabhi nodes ki distance infinity aur previous node -1 set kar rahe hain
    for (i = 0; i < graph->node_count; i++)
    {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    // Source se source ki distance zero hoti hai
    distances[source] = 0.0;

    // Har node ko maximum ek baar process karenge
    for (count = 0; count < graph->node_count; count++)
    {
        // Abhi koi node select nahi hua
        int current = -1;

        // Minimum distance initially infinity rakhi hai
        double minimum = DBL_MAX;

        // Sabhi nodes mein se minimum distance wala unvisited node find kar rahe hain
        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] &&
                distances[i] < minimum)
            {
                // Naya minimum distance mil gaya
                minimum = distances[i];

                // Is node ko current node select kar liya
                current = i;
            }
        }

        // Agar koi reachable unvisited node nahi mila toh algorithm stop
        if (current == -1)
            break;

        // Current node ko processed mark kar diya
        visited[current] = 1;

        // Agar destination mil gaya toh aage process karne ki zarurat nahi
        if (current == destination)
            break;

        // Current node ke edges ki count valid hai ya nahi check kar rahe hain
        if (graph->nodes[current].edge_count < 0 ||
            graph->nodes[current].edge_count > MAX_NODES)
        {
            return 0;
        }

        // Current node ke saare neighbouring nodes check karenge
        for (i = 0;
             i < graph->nodes[current].edge_count;
             i++)
        {
            // Neighbour node ka index
            int next =
                graph->nodes[current].edges[i].destination;

            // Current node se neighbour tak road ki distance
            double distance =
                graph->nodes[current].edges[i].distance;

            // Check kar rahe hain ki edge ka data valid hai
            if (next < 0 || next >= graph->node_count ||
                !isfinite(distance) || distance < 0)
            {
                return 0;
            }

            // Check kar rahe hain ki neighbour tak koi shorter path mil sakta hai
            if (!visited[next] &&
                distances[current] + distance < distances[next])
            {
                // Shorter distance milne par distance update kar do
                distances[next] =
                    distances[current] + distance;

                // Shortest path mein current node, next ka previous node hoga
                previous[next] = current;
            }
        }
    }

    // Algorithm successfully execute ho gaya
    return 1;
}


/* Dijkstra: ek source se saari locations tak shortest distance */

int dijkstra_all(const Graph *graph,
                 int source,
                 double distances[MAX_NODES],
                 int previous[MAX_NODES])
{
    // Track karenge ki kaunse nodes process ho chuke hain
    int visited[MAX_NODES] = {0};
    int i, count;

    // Required pointers valid hone chahiye
    if (graph == NULL ||
        distances == NULL ||
        previous == NULL)
    {
        return 0;
    }

    // Graph mein nodes ki count valid honi chahiye
    if (graph->node_count <= 0 ||
        graph->node_count > MAX_NODES)
    {
        return 0;
    }

    // Source node graph ke andar exist karna chahiye
    if (source < 0 || source >= graph->node_count)
    {
        return 0;
    }

    // Sabhi nodes ki distance infinity aur previous -1 initialize kar rahe hain
    for (i = 0; i < graph->node_count; i++)
    {
        distances[i] = DBL_MAX;
        previous[i] = -1;
    }

    // Source ki distance zero set kar di
    distances[source] = 0.0;

    // Har iteration mein ek minimum distance unvisited node process hoga
    for (count = 0; count < graph->node_count; count++)
    {
        // Current node abhi select nahi hua
        int current = -1;

        // Minimum distance initially infinity
        double minimum = DBL_MAX;

        // Minimum distance wala unvisited node find kar rahe hain
        for (i = 0; i < graph->node_count; i++)
        {
            if (!visited[i] &&
                distances[i] < minimum)
            {
                // Minimum distance update
                minimum = distances[i];

                // Current node select
                current = i;
            }
        }

        // Agar koi reachable node nahi bacha toh stop
        if (current == -1)
            break;

        // Current node ko processed mark kar do
        visited[current] = 1;

        // Current node ke edges ki count validate kar rahe hain
        if (graph->nodes[current].edge_count < 0 ||
            graph->nodes[current].edge_count > MAX_NODES)
        {
            return 0;
        }

        // Current node ke neighbours check kar rahe hain
        for (i = 0;
             i < graph->nodes[current].edge_count;
             i++)
        {
            // Neighbour ka index
            int next =
                graph->nodes[current].edges[i].destination;

            // Neighbour tak road ki distance
            double distance =
                graph->nodes[current].edges[i].distance;

            // Edge ka data valid hai ya nahi check
            if (next < 0 || next >= graph->node_count ||
                !isfinite(distance) || distance < 0)
            {
                return 0;
            }

            // Agar shorter path milta hai aur neighbour process nahi hua
            if (!visited[next] &&
                distances[current] + distance < distances[next])
            {
                // Neighbour ki shortest distance update
                distances[next] =
                    distances[current] + distance;

                // Path reconstruct karne ke liye previous node save
                previous[next] = current;
            }
        }
    }

    // Algorithm successfully execute ho gaya
    return 1;
}