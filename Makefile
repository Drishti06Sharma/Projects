CC = gcc
CFLAGS = -Wall -Wextra -std=c11

CLI_TARGET = ev_planner.exe

CLI_SOURCES = main.c \
              graph.c \
              dijkstra.c \
              fastest_route.c \
              bellman_ford.c \
              ev_planner.c \
              input_validation.c \
              advanced_algorithms.c \
              station_comparison.c \
              stress_benchmark.c \
              knapsack.c \
              prim.c \
              kruskal.c

all: cli

cli: $(CLI_TARGET)

$(CLI_TARGET): $(CLI_SOURCES)
	$(CC) $(CFLAGS) $(CLI_SOURCES) -o $(CLI_TARGET)

run: $(CLI_TARGET)
	./$(CLI_TARGET)

clean:
	cmd /C "if exist $(CLI_TARGET) del /Q $(CLI_TARGET)"
	cmd /C "if exist *.o del /Q *.o"

.PHONY: all cli run clean