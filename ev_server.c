#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#include <math.h>

#include "graph.h"
#include "dijkstra.h"
#include "fastest_route.h"
#include "bellman_ford.h"
#include "advanced_algorithms.h"
#include "station_comparison.h"
#include "stress_benchmark.h"

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 65536
#define MAX_JSON_RESP (128 * 1024)

static Graph active_graph;

/* Default layout coordinates for Dehradun map rendering */
typedef struct {
    int x;
    int y;
} LocationCoord;

static LocationCoord default_coords[10] = {
    {500, 380}, /* 0: Clock Tower (Center) */
    {240, 560}, /* 1: ISBT Dehradun (South-West) */
    {420, 470}, /* 2: Railway Station (South) */
    {230, 350}, /* 3: Forest Research Institute (West) */
    {650, 250}, /* 4: Pacific Mall (North-East, Rajpur Rd) */
    {100, 280}, /* 5: Prem Nagar (Far West) */
    {720, 110}, /* 6: DIT University (Far North Foothills) */
    {620, 170}, /* 7: Max Hospital (North-East) */
    {570, 410}, /* 8: Survey Chowk (East of Clock Tower) */
    {710, 340}  /* 9: Sahastradhara Road (East) */
};

/* Utility to parse integer parameter from query string or JSON */
static int get_int_param(const char *body, const char *key, int default_val) {
    char search_pattern[128];
    snprintf(search_pattern, sizeof(search_pattern), "\"%s\":", key);
    char *pos = strstr(body, search_pattern);
    if (!pos) {
        snprintf(search_pattern, sizeof(search_pattern), "%s=", key);
        pos = strstr(body, search_pattern);
        if (!pos) return default_val;
        return atoi(pos + strlen(search_pattern));
    }
    return atoi(pos + strlen(search_pattern));
}

/* Utility to parse double parameter */
static double get_double_param(const char *body, const char *key, double default_val) {
    char search_pattern[128];
    snprintf(search_pattern, sizeof(search_pattern), "\"%s\":", key);
    char *pos = strstr(body, search_pattern);
    if (!pos) {
        snprintf(search_pattern, sizeof(search_pattern), "%s=", key);
        pos = strstr(body, search_pattern);
        if (!pos) return default_val;
        return atof(pos + strlen(search_pattern));
    }
    return atof(pos + strlen(search_pattern));
}

/* Utility to parse string parameter */
static void get_string_param(const char *body, const char *key, char *out, size_t max_len) {
    out[0] = '\0';
    char search_pattern[128];
    snprintf(search_pattern, sizeof(search_pattern), "\"%s\":\"", key);
    char *pos = strstr(body, search_pattern);
    if (pos) {
        pos += strlen(search_pattern);
        char *end = strchr(pos, '\"');
        if (end) {
            size_t len = (size_t)(end - pos);
            if (len >= max_len) len = max_len - 1;
            strncpy(out, pos, len);
            out[len] = '\0';
            return;
        }
    }
    snprintf(search_pattern, sizeof(search_pattern), "%s=", key);
    pos = strstr(body, search_pattern);
    if (pos) {
        pos += strlen(search_pattern);
        char *end = strpbrk(pos, " &\r\n");
        size_t len = end ? (size_t)(end - pos) : strlen(pos);
        if (len >= max_len) len = max_len - 1;
        strncpy(out, pos, len);
        out[len] = '\0';
    }
}

/* Helper to read static file into memory */
static char *read_static_file(const char *filepath, size_t *out_len) {
    FILE *f = fopen(filepath, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) {
        fclose(f);
        return NULL;
    }
    char *buf = (char *)malloc(sz + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t read_bytes = fread(buf, 1, sz, f);
    buf[read_bytes] = '\0';
    fclose(f);
    if (out_len) *out_len = read_bytes;
    return buf;
}

/* Generate JSON for the entire network graph */
static void handle_api_network(char *resp_buf, size_t max_len) {
    char *p = resp_buf;
    size_t rem = max_len;
    int written = snprintf(p, rem,
        "{\"success\":true,\"node_count\":%d,\"nodes\":[",
        active_graph.node_count);
    p += written; rem -= written;

    for (int i = 0; i < active_graph.node_count; i++) {
        int x = (i < 10) ? default_coords[i].x : 200 + (i * 60) % 600;
        int y = (i < 10) ? default_coords[i].y : 150 + (i * 45) % 400;

        written = snprintf(p, rem,
            "%s{\"id\":%d,\"name\":\"%s\",\"edge_count\":%d,\"has_station\":%d,\"charger_count\":%d,\"installation_cost\":%.2f,\"x\":%d,\"y\":%d}",
            (i > 0 ? "," : ""),
            i, active_graph.nodes[i].name,
            active_graph.nodes[i].edge_count,
            active_graph.nodes[i].has_station,
            active_graph.nodes[i].charger_count,
            active_graph.nodes[i].installation_cost,
            x, y);
        p += written; rem -= written;
    }

    written = snprintf(p, rem, "],\"edges\":[");
    p += written; rem -= written;

    int edge_idx = 0;
    for (int i = 0; i < active_graph.node_count; i++) {
        for (int j = 0; j < active_graph.nodes[i].edge_count; j++) {
            int dest = active_graph.nodes[i].edges[j].destination;
            /* In an undirected graph, only output each edge once (source < destination) */
            if (i < dest) {
                written = snprintf(p, rem,
                    "%s{\"source\":%d,\"source_name\":\"%s\",\"destination\":%d,\"dest_name\":\"%s\",\"distance\":%.2f,\"time\":%.2f}",
                    (edge_idx > 0 ? "," : ""),
                    i, active_graph.nodes[i].name,
                    dest, active_graph.nodes[dest].name,
                    active_graph.nodes[i].edges[j].distance,
                    active_graph.nodes[i].edges[j].time);
                p += written; rem -= written;
                edge_idx++;
            }
        }
    }

    snprintf(p, rem, "]}");
}

/* Handle route calculation using Dijkstra, Fastest Route, or Bellman-Ford */
static void handle_api_route(const char *req_data, char *resp_buf, size_t max_len) {
    int source = get_int_param(req_data, "source", 0);
    int destination = get_int_param(req_data, "destination", 6);
    char algo[32] = {0};
    get_string_param(req_data, "algorithm", algo, sizeof(algo));
    if (algo[0] == '\0') strcpy(algo, "dijkstra");

    if (source < 0 || source >= active_graph.node_count ||
        destination < 0 || destination >= active_graph.node_count) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Invalid source or destination location ID.\"}");
        return;
    }

    double distances[MAX_NODES];
    double times[MAX_NODES];
    int previous[MAX_NODES];
    int success = 0;

    LARGE_INTEGER freq, t_start, t_end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t_start);

    if (strcmp(algo, "fastest") == 0) {
        success = fastest_route(&active_graph, source, destination, times, previous);
    } else if (strcmp(algo, "bellman_ford") == 0) {
        int bf_res = bellman_ford(&active_graph, source, distances, previous);
        if (bf_res == -1) {
            snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Negative-weight cycle detected!\"}");
            return;
        }
        success = (bf_res == 1);
    } else {
        /* Default to shortest distance Dijkstra */
        strcpy(algo, "dijkstra");
        success = dijkstra(&active_graph, source, destination, distances, previous);
    }

    QueryPerformanceCounter(&t_end);
    double elapsed_ms = (double)(t_end.QuadPart - t_start.QuadPart) * 1000.0 / (double)freq.QuadPart;

    if (!success) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Algorithm execution failed.\"}");
        return;
    }

    /* Check reachability */
    if (strcmp(algo, "fastest") == 0) {
        if (times[destination] >= DBL_MAX - 1000.0) {
            snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"No path exists between %s and %s.\"}",
                     active_graph.nodes[source].name, active_graph.nodes[destination].name);
            return;
        }
    } else {
        if (distances[destination] >= DBL_MAX - 1000.0) {
            snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"No path exists between %s and %s.\"}",
                     active_graph.nodes[source].name, active_graph.nodes[destination].name);
            return;
        }
    }

    /* Reconstruct path backwards from destination */
    int path[MAX_NODES];
    int path_len = 0;
    int curr = destination;
    while (curr != -1 && path_len < MAX_NODES) {
        path[path_len++] = curr;
        if (curr == source) break;
        curr = previous[curr];
    }

    /* Reverse to source -> destination */
    for (int i = 0; i < path_len / 2; i++) {
        int tmp = path[i];
        path[i] = path[path_len - 1 - i];
        path[path_len - 1 - i] = tmp;
    }

    /* Calculate accurate cumulative distance, travel time, and intermediate hops */
    double total_dist = 0.0;
    double total_time = 0.0;

    char *p = resp_buf;
    size_t rem = max_len;
    int written = snprintf(p, rem,
        "{\"success\":true,\"algorithm\":\"%s\",\"source\":%d,\"source_name\":\"%s\",\"destination\":%d,\"destination_name\":\"%s\",\"execution_time_ms\":%.6f,\"path\":[",
        algo, source, active_graph.nodes[source].name,
        destination, active_graph.nodes[destination].name,
        elapsed_ms);
    p += written; rem -= written;

    for (int i = 0; i < path_len; i++) {
        written = snprintf(p, rem, "%s%d", (i > 0 ? "," : ""), path[i]);
        p += written; rem -= written;
    }

    written = snprintf(p, rem, "],\"path_names\":[");
    p += written; rem -= written;

    for (int i = 0; i < path_len; i++) {
        written = snprintf(p, rem, "%s\"%s\"", (i > 0 ? "," : ""), active_graph.nodes[path[i]].name);
        p += written; rem -= written;
    }

    written = snprintf(p, rem, "],\"hops\":[");
    p += written; rem -= written;

    for (int i = 0; i < path_len - 1; i++) {
        int u = path[i];
        int v = path[i + 1];
        double edge_d = 0.0;
        double edge_t = 0.0;

        for (int k = 0; k < active_graph.nodes[u].edge_count; k++) {
            if (active_graph.nodes[u].edges[k].destination == v) {
                edge_d = active_graph.nodes[u].edges[k].distance;
                edge_t = active_graph.nodes[u].edges[k].time;
                break;
            }
        }
        total_dist += edge_d;
        total_time += edge_t;

        written = snprintf(p, rem,
            "%s{\"from\":%d,\"from_name\":\"%s\",\"to\":%d,\"to_name\":\"%s\",\"distance\":%.2f,\"time\":%.2f}",
            (i > 0 ? "," : ""),
            u, active_graph.nodes[u].name,
            v, active_graph.nodes[v].name,
            edge_d, edge_t);
        p += written; rem -= written;
    }

    /* Include stations along this route */
    written = snprintf(p, rem, "],\"stations_on_route\":[");
    p += written; rem -= written;

    int st_count = 0;
    for (int i = 0; i < path_len; i++) {
        int u = path[i];
        if (active_graph.nodes[u].has_station) {
            written = snprintf(p, rem,
                "%s{\"id\":%d,\"name\":\"%s\",\"chargers\":%d,\"cost\":%.2f}",
                (st_count > 0 ? "," : ""),
                u, active_graph.nodes[u].name,
                active_graph.nodes[u].charger_count,
                active_graph.nodes[u].installation_cost);
            p += written; rem -= written;
            st_count++;
        }
    }

    const char *time_comp = "O(V^2 + E)";
    const char *space_comp = "O(V)";
    if (strcmp(algo, "bellman_ford") == 0) {
        time_comp = "O(V * E)";
    }

    snprintf(p, rem,
        "],\"total_distance\":%.2f,\"total_time\":%.2f,\"time_formatted\":\"%d hrs %d mins\",\"time_complexity\":\"%s\",\"space_complexity\":\"%s\"}",
        total_dist, total_time,
        (int)total_time, (int)((total_time - (int)total_time) * 60.0 + 0.5),
        time_comp, space_comp);
}

/* Install charging station at a location */
static void handle_api_install_station(const char *req_data, char *resp_buf, size_t max_len) {
    int loc_id = get_int_param(req_data, "location_id", -1);
    int chargers = get_int_param(req_data, "charger_count", 4);
    double cost = get_double_param(req_data, "installation_cost", 1000.0);

    if (loc_id < 0 || loc_id >= active_graph.node_count) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Invalid location ID.\"}");
        return;
    }
    if (chargers <= 0 || cost <= 0) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Chargers and cost must be positive.\"}");
        return;
    }
    if (active_graph.nodes[loc_id].has_station) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"A charging station already exists at %s.\"}",
                 active_graph.nodes[loc_id].name);
        return;
    }

    if (install_charging_station(&active_graph, loc_id, chargers, cost)) {
        snprintf(resp_buf, max_len,
            "{\"success\":true,\"message\":\"Charging station installed at %s successfully!\",\"location_id\":%d,\"location_name\":\"%s\",\"chargers\":%d,\"cost\":%.2f}",
            active_graph.nodes[loc_id].name, loc_id, active_graph.nodes[loc_id].name, chargers, cost);
    } else {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Could not install charging station.\"}");
    }
}

/* Automatic Greedy Charging Station Placement */
static void handle_api_auto_station(const char *req_data, char *resp_buf, size_t max_len) {
    double budget = get_double_param(req_data, "budget", 5000.0);
    double cost = get_double_param(req_data, "cost", 1000.0);
    int chargers = get_int_param(req_data, "chargers", 4);

    if (budget <= 0 || cost <= 0 || chargers <= 0) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Budget, cost, and chargers must be positive values.\"}");
        return;
    }

    int placed = 0;
    double spent = 0.0;
    int placed_nodes[MAX_NODES];

    while (spent + cost <= budget) {
        int best = -1;

        for (int i = 0; i < active_graph.node_count; i++) {
            if (active_graph.nodes[i].has_station)
                continue;

            if (best == -1 || active_graph.nodes[i].edge_count > active_graph.nodes[best].edge_count) {
                best = i;
            }
        }

        if (best == -1) break;

        if (!install_charging_station(&active_graph, best, chargers, cost)) {
            break;
        }

        placed_nodes[placed] = best;
        spent += cost;
        placed++;
    }

    char *p = resp_buf;
    size_t rem = max_len;
    int written = snprintf(p, rem,
        "{\"success\":true,\"placed_count\":%d,\"total_spent\":%.2f,\"remaining_budget\":%.2f,\"installed_stations\":[",
        placed, spent, budget - spent);
    p += written; rem -= written;

    for (int i = 0; i < placed; i++) {
        int id = placed_nodes[i];
        written = snprintf(p, rem,
            "%s{\"id\":%d,\"name\":\"%s\",\"connections\":%d,\"chargers\":%d,\"cost\":%.2f}",
            (i > 0 ? "," : ""),
            id, active_graph.nodes[id].name,
            active_graph.nodes[id].edge_count,
            active_graph.nodes[id].charger_count,
            active_graph.nodes[id].installation_cost);
        p += written; rem -= written;
    }

    snprintf(p, rem, "]}");
}

/* Compare Connectivity-Based vs Demand-Based Placement Strategies */
static void handle_api_compare_strategies(const char *req_data, char *resp_buf, size_t max_len) {
    double budget = get_double_param(req_data, "budget", 4000.0);
    double station_cost = get_double_param(req_data, "station_cost", 1000.0);
    int chargers_per_station = get_int_param(req_data, "chargers_per_station", 10);
    int maximum_chargers = get_int_param(req_data, "maximum_chargers", 50);

    /* Demand scores for each location */
    double demand[MAX_NODES];
    /* Default realistic priority scores for Dehradun landmarks */
    double default_demand[10] = {95.0, 90.0, 85.0, 60.0, 88.0, 70.0, 82.0, 75.0, 78.0, 68.0};
    for (int i = 0; i < active_graph.node_count; i++) {
        char key[32];
        snprintf(key, sizeof(key), "demand_%d", i);
        double d = get_double_param(req_data, key, (i < 10 ? default_demand[i] : 50.0));
        demand[i] = (d > 0) ? d : 1.0;
    }

    if (budget <= 0 || station_cost <= 0 || chargers_per_station <= 0 || maximum_chargers <= 0) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Constraints must be positive numbers.\"}");
        return;
    }

    /* Calculate max stations allowed under dual constraints */
    int max_stations = (int)(budget / station_cost);
    if (max_stations > maximum_chargers / chargers_per_station)
        max_stations = maximum_chargers / chargers_per_station;
    if (max_stations > active_graph.node_count)
        max_stations = active_graph.node_count;
    if (max_stations < 0) max_stations = 0;

    int selected_a[MAX_NODES] = {0};
    int selected_b[MAX_NODES] = {0};

    int count_a = run_greedy_strategy(&active_graph, demand, selected_a, 1, max_stations);
    int count_b = run_greedy_strategy(&active_graph, demand, selected_b, 2, max_stations);

    double score_a = 0.0, score_b = 0.0;
    for (int i = 0; i < active_graph.node_count; i++) {
        if (selected_a[i]) score_a += demand[i];
        if (selected_b[i]) score_b += demand[i];
    }

    char *p = resp_buf;
    size_t rem = max_len;
    int written = snprintf(p, rem,
        "{\"success\":true,\"max_stations\":%d,\"budget\":%.2f,\"station_cost\":%.2f,\"chargers_per_station\":%d,\"maximum_chargers\":%d,"
        "\"strategy_a\":{\"name\":\"Connectivity-Based Greedy\",\"count\":%d,\"total_chargers\":%d,\"total_cost\":%.2f,\"captured_score\":%.2f,\"stations\":[",
        max_stations, budget, station_cost, chargers_per_station, maximum_chargers,
        count_a, count_a * chargers_per_station, count_a * station_cost, score_a);
    p += written; rem -= written;

    int st_idx = 0;
    for (int i = 0; i < active_graph.node_count; i++) {
        if (selected_a[i]) {
            written = snprintf(p, rem,
                "%s{\"id\":%d,\"name\":\"%s\",\"connections\":%d,\"demand_score\":%.2f}",
                (st_idx > 0 ? "," : ""),
                i, active_graph.nodes[i].name, active_graph.nodes[i].edge_count, demand[i]);
            p += written; rem -= written;
            st_idx++;
        }
    }

    written = snprintf(p, rem,
        "]},\"strategy_b\":{\"name\":\"Demand-Based Greedy\",\"count\":%d,\"total_chargers\":%d,\"total_cost\":%.2f,\"captured_score\":%.2f,\"stations\":[",
        count_b, count_b * chargers_per_station, count_b * station_cost, score_b);
    p += written; rem -= written;

    st_idx = 0;
    for (int i = 0; i < active_graph.node_count; i++) {
        if (selected_b[i]) {
            written = snprintf(p, rem,
                "%s{\"id\":%d,\"name\":\"%s\",\"connections\":%d,\"demand_score\":%.2f}",
                (st_idx > 0 ? "," : ""),
                i, active_graph.nodes[i].name, active_graph.nodes[i].edge_count, demand[i]);
            p += written; rem -= written;
            st_idx++;
        }
    }

    snprintf(p, rem, "]}}");
}

/* Run Synthetic Stress Benchmark */
static void handle_api_stress_benchmark(const char *req_data, char *resp_buf, size_t max_len) {
    int repetitions = get_int_param(req_data, "repetitions", 5000);
    if (repetitions <= 0) repetitions = 5000;
    if (repetitions > 50000) repetitions = 50000;

    StressBenchmarkResult res;
    if (!execute_stress_benchmark(repetitions, &res)) {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Stress benchmark execution failed.\"}");
        return;
    }

    snprintf(resp_buf, max_len,
        "{\"success\":true,\"node_count\":%d,\"edge_count\":%d,\"repetitions\":%d,"
        "\"dijkstra_time_ms\":%.6f,\"bellman_time_ms\":%.6f,\"distances_match\":%s,"
        "\"graph_memory_bytes\":%lu,\"distance_arrays_bytes\":%lu,\"dijkstra_visited_bytes\":%lu,"
        "\"dijkstra_complexity\":\"O(V^2 + E)\",\"bellman_complexity\":\"O(V * E)\"}",
        res.node_count, res.edge_count, res.repetitions,
        res.dijkstra_time_ms, res.bellman_time_ms,
        res.distances_match ? "true" : "false",
        res.graph_memory, res.distance_array_memory, res.dijkstra_visited_memory);
}

/* Quick Sort and Binary Search Module */
static void handle_api_sort_search(const char *req_data, char *resp_buf, size_t max_len) {
    int key = get_int_param(req_data, "key", 23);
    char sort_algo[32] = {0};
    get_string_param(req_data, "sort_algo", sort_algo, sizeof(sort_algo));
    if (sort_algo[0] == '\0') strcpy(sort_algo, "quick");

    char search_algo[32] = {0};
    get_string_param(req_data, "search_algo", search_algo, sizeof(search_algo));
    if (search_algo[0] == '\0') strcpy(search_algo, "iterative");

    /* Default array */
    int arr[64] = {45, 12, 78, 23, 9, 56, 34, 89, 15, 67};
    int n = 10;

    /* Parse custom array if provided in JSON: "array":[...] */
    char *arr_pos = strstr(req_data, "\"array\":[");
    if (arr_pos) {
        arr_pos += 9;
        n = 0;
        while (*arr_pos && *arr_pos != ']' && n < 64) {
            while (*arr_pos == ' ' || *arr_pos == ',') arr_pos++;
            if (*arr_pos == ']' || *arr_pos == '\0') break;
            arr[n++] = atoi(arr_pos);
            while (*arr_pos && *arr_pos != ',' && *arr_pos != ']') arr_pos++;
        }
    }

    if (n == 0) {
        arr[0] = 45; arr[1] = 12; arr[2] = 78; arr[3] = 23; arr[4] = 9; arr[5] = 56;
        n = 6;
    }

    int orig[64];
    for (int i = 0; i < n; i++) orig[i] = arr[i];

    LARGE_INTEGER freq, t_start, t_end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t_start);

    if (strcmp(sort_algo, "merge") == 0) {
        merge_sort(arr, 0, n - 1);
    } else {
        strcpy(sort_algo, "quick");
        quick_sort(arr, 0, n - 1);
    }

    QueryPerformanceCounter(&t_end);
    double sort_time_us = (double)(t_end.QuadPart - t_start.QuadPart) * 1000000.0 / (double)freq.QuadPart;

    int search_index = -1;
    if (strcmp(search_algo, "recursive") == 0) {
        search_index = binary_search_recursive(arr, 0, n - 1, key);
    } else {
        strcpy(search_algo, "iterative");
        search_index = binary_search_iterative(arr, n, key);
    }

    char *p = resp_buf;
    size_t rem = max_len;
    int written = snprintf(p, rem,
        "{\"success\":true,\"sort_algo\":\"%s\",\"search_algo\":\"%s\",\"key\":%d,\"search_index\":%d,\"sort_time_us\":%.3f,\"original\":[",
        sort_algo, search_algo, key, search_index, sort_time_us);
    p += written; rem -= written;

    for (int i = 0; i < n; i++) {
        written = snprintf(p, rem, "%s%d", (i > 0 ? "," : ""), orig[i]);
        p += written; rem -= written;
    }

    written = snprintf(p, rem, "],\"sorted\":[");
    p += written; rem -= written;

    for (int i = 0; i < n; i++) {
        written = snprintf(p, rem, "%s%d", (i > 0 ? "," : ""), arr[i]);
        p += written; rem -= written;
    }

    snprintf(p, rem, "],\"sort_complexity\":\"O(N log N)\",\"search_complexity\":\"O(log N)\"}");
}

/* Reset graph to default Dehradun network */
static void handle_api_reset_network(char *resp_buf, size_t max_len) {
    initialize_graph(&active_graph);
    snprintf(resp_buf, max_len, "{\"success\":true,\"message\":\"Dehradun road network reset to default successfully!\"}");
}

/* Add custom location */
static void handle_api_add_location(const char *req_data, char *resp_buf, size_t max_len) {
    char name[MAX_NAME];
    get_string_param(req_data, "name", name, sizeof(name));
    if (name[0] == '\0') {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Location name cannot be empty.\"}");
        return;
    }
    int id = add_location(&active_graph, name);
    if (id >= 0) {
        snprintf(resp_buf, max_len, "{\"success\":true,\"message\":\"Location added successfully!\",\"id\":%d,\"name\":\"%s\"}", id, name);
    } else {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Could not add location. Check for duplicates or max node limit.\"}");
    }
}

/* Add custom road */
static void handle_api_add_road(const char *req_data, char *resp_buf, size_t max_len) {
    int src = get_int_param(req_data, "source", -1);
    int dst = get_int_param(req_data, "destination", -1);
    double dist = get_double_param(req_data, "distance", 0.0);
    double time = get_double_param(req_data, "time", 0.0);

    if (add_road(&active_graph, src, dst, dist, time)) {
        snprintf(resp_buf, max_len, "{\"success\":true,\"message\":\"Road added successfully!\"}");
    } else {
        snprintf(resp_buf, max_len, "{\"success\":false,\"error\":\"Could not add road. Check IDs, distance, and time.\"}");
    }
}

/* Dispatch incoming HTTP request */
static void handle_client_request(SOCKET client_sock) {
    char req_buf[BUFFER_SIZE];
    int recv_bytes = recv(client_sock, req_buf, sizeof(req_buf) - 1, 0);
    if (recv_bytes <= 0) return;
    req_buf[recv_bytes] = '\0';

    char method[16] = {0};
    char path[256] = {0};
    sscanf(req_buf, "%15s %255s", method, path);

    /* Clean query parameters from path for file routing */
    char pure_path[256];
    strncpy(pure_path, path, sizeof(pure_path) - 1);
    pure_path[sizeof(pure_path) - 1] = '\0';
    char *qmark = strchr(pure_path, '?');
    if (qmark) *qmark = '\0';

    /* Handle CORS preflight */
    if (strcmp(method, "OPTIONS") == 0) {
        const char *cors_resp =
            "HTTP/1.1 204 No Content\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
            "Access-Control-Allow-Headers: Content-Type, Accept\r\n"
            "Access-Control-Max-Age: 86400\r\n\r\n";
        send(client_sock, cors_resp, (int)strlen(cors_resp), 0);
        return;
    }

    /* Locate HTTP request body for POST requests */
    char *body = strstr(req_buf, "\r\n\r\n");
    if (body) body += 4;
    else body = req_buf;

    /* Combine path query string with body for flexible param extraction */
    char combined_params[BUFFER_SIZE];
    snprintf(combined_params, sizeof(combined_params), "%s\n%s", path, body);

    /* Allocate response buffer */
    char *resp_payload = (char *)malloc(MAX_JSON_RESP);
    if (!resp_payload) return;
    resp_payload[0] = '\0';

    const char *content_type = "application/json";

    /* API Endpoint Router */
    if (strncmp(pure_path, "/api/network", 12) == 0) {
        handle_api_network(resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/route", 10) == 0) {
        handle_api_route(combined_params, resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/install_station", 20) == 0) {
        handle_api_install_station(combined_params, resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/auto_station", 17) == 0) {
        handle_api_auto_station(combined_params, resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/compare_strategies", 23) == 0) {
        handle_api_compare_strategies(combined_params, resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/stress_benchmark", 21) == 0) {
        handle_api_stress_benchmark(combined_params, resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/sort_search", 16) == 0) {
        handle_api_sort_search(combined_params, resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/reset_network", 18) == 0) {
        handle_api_reset_network(resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/add_location", 17) == 0) {
        handle_api_add_location(combined_params, resp_payload, MAX_JSON_RESP);
    } else if (strncmp(pure_path, "/api/add_road", 13) == 0) {
        handle_api_add_road(combined_params, resp_payload, MAX_JSON_RESP);
    } else {
        /* Serve Static Frontend Files */
        char filepath[512];
        if (strcmp(pure_path, "/") == 0 || strcmp(pure_path, "/index.html") == 0) {
            snprintf(filepath, sizeof(filepath), "gui/index.html");
            content_type = "text/html; charset=utf-8";
        } else if (strcmp(pure_path, "/style.css") == 0 || strcmp(pure_path, "/gui/style.css") == 0) {
            snprintf(filepath, sizeof(filepath), "gui/style.css");
            content_type = "text/css; charset=utf-8";
        } else if (strcmp(pure_path, "/app.js") == 0 || strcmp(pure_path, "/gui/app.js") == 0) {
            snprintf(filepath, sizeof(filepath), "gui/app.js");
            content_type = "application/javascript; charset=utf-8";
        } else {
            snprintf(filepath, sizeof(filepath), "gui%s", pure_path);
            if (strstr(pure_path, ".css")) content_type = "text/css";
            else if (strstr(pure_path, ".js")) content_type = "application/javascript";
            else content_type = "text/html";
        }

        size_t file_len = 0;
        char *file_data = read_static_file(filepath, &file_len);
        if (file_data) {
            char header[512];
            int hdr_len = snprintf(header, sizeof(header),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %zu\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Connection: close\r\n\r\n",
                content_type, file_len);
            send(client_sock, header, hdr_len, 0);
            send(client_sock, file_data, (int)file_len, 0);
            free(file_data);
            free(resp_payload);
            return;
        } else {
            snprintf(resp_payload, MAX_JSON_RESP, "{\"error\":\"404 Not Found: %s\"}", pure_path);
            content_type = "application/json";
        }
    }

    /* Send JSON API response */
    size_t payload_len = strlen(resp_payload);
    char header[512];
    int hdr_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n\r\n",
        content_type, payload_len);

    send(client_sock, header, hdr_len, 0);
    send(client_sock, resp_payload, (int)payload_len, 0);
    free(resp_payload);
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0) port = DEFAULT_PORT;
    }

    printf("============================================================\n");
    printf("   EV TRIP PLANNER - NATIVE C GUI BACKEND SERVER\n");
    printf("   CSEG3053: Design and Analysis of Algorithms\n");
    printf("============================================================\n");

    /* Initialize graph with default Dehradun network */
    initialize_graph(&active_graph);
    printf("[OK] Dehradun Road Network loaded successfully (%d locations)\n", active_graph.node_count);

    /* Initialize Winsock */
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[ERROR] Failed to initialize Winsock (code: %d)\n", WSAGetLastError());
        return 1;
    }

    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock == INVALID_SOCKET) {
        printf("[ERROR] Failed to create socket (code: %d)\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    /* Allow address reuse */
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((u_short)port);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("[ERROR] Port %d is busy. Trying port %d...\n", port, port + 1);
        port++;
        server_addr.sin_port = htons((u_short)port);
        if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            printf("[ERROR] Socket bind failed (code: %d)\n", WSAGetLastError());
            closesocket(server_sock);
            WSACleanup();
            return 1;
        }
    }

    if (listen(server_sock, SOMAXCONN) == SOCKET_ERROR) {
        printf("[ERROR] Socket listen failed (code: %d)\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }

    printf("[SERVER] Running at: http://localhost:%d/\n", port);
    printf("[SERVER] REST API is live and directly invoking C algorithms in memory.\n");
    printf("[SERVER] Press Ctrl+C in this console to shut down the server.\n");
    printf("------------------------------------------------------------\n");

    /* Automatically open browser on Windows if flag not disabled */
    char open_cmd[256];
    snprintf(open_cmd, sizeof(open_cmd), "start http://localhost:%d/", port);
    if (argc <= 2 || strcmp(argv[2], "--no-open") != 0) {
        system(open_cmd);
    }

    while (1) {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        SOCKET client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock == INVALID_SOCKET) {
            continue;
        }

        handle_client_request(client_sock);
        closesocket(client_sock);
    }

    closesocket(server_sock);
    WSACleanup();
    return 0;
}
