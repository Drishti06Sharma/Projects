
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = ev_planner

SOURCES = main.c graph.c dijkstra.c bellman_ford.c ev_planner.c input_validation.c

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
