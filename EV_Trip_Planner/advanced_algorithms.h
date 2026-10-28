
#ifndef ADVANCED_ALGORITHMS_H
#define ADVANCED_ALGORITHMS_H

void quick_sort(int arr[], int low, int high);
void merge_sort(int arr[], int left, int right);

int binary_search_iterative(int arr[], int n, int key);

int binary_search_recursive(
    int arr[], int low, int high, int key
);

void advanced_algorithms_menu(void);

#endif
