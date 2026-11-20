CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lws2_32

CLI_TARGET = ev_planner.exe
GUI_TARGET = ev_server.exe

CLI_SOURCES = main.c graph.c dijkstra.c fastest_route.c bellman_ford.c ev_planner.c input_validation.c advanced_algorithms.c station_comparison.c stress_benchmark.c knapsack.c

GUI_SOURCES = ev_server.c graph.c dijkstra.c fastest_route.c bellman_ford.c advanced_algorithms.c station_comparison.c stress_benchmark.c

all: $(CLI_TARGET) $(GUI_TARGET)

cli: $(CLI_TARGET)

gui: $(GUI_TARGET)

$(CLI_TARGET): $(CLI_SOURCES)
	$(CC) $(CFLAGS) $(CLI_SOURCES) -o $(CLI_TARGET)

$(GUI_TARGET): $(GUI_SOURCES)
	$(CC) $(CFLAGS) $(GUI_SOURCES) $(LIBS) -o $(GUI_TARGET)

clean:
	rm -f $(CLI_TARGET) $(GUI_TARGET) *.o