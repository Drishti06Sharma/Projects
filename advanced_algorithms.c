
#include <stdio.h>
#include "advanced_algorithms.h"
#include "input_validation.h"

/* ---------- QUICK SORT ---------- */

void quick_sort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (arr[j] <= pivot)
            {
                i++;

                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pivot_index = i + 1;

        quick_sort(arr, low, pivot_index - 1);
        quick_sort(arr, pivot_index + 1, high);
    }
}

/* ---------- ITERATIVE BINARY SEARCH ---------- */

int binary_search_iterative(int arr[], int n, int key)
{
    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid] == key)
        {
            return mid;
        }

        if (arr[mid] < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1;
}

/* ---------- RECURSIVE BINARY SEARCH ---------- */

int binary_search_recursive(int arr[], int low, int high, int key)
{
    if (low > high)
    {
        return -1;
    }

    int mid = low + (high - low) / 2;

    if (arr[mid] == key)
    {
        return mid;
    }

    if (arr[mid] < key)
    {
        return binary_search_recursive(
            arr, mid + 1, high, key);
    }

    return binary_search_recursive(
        arr, low, mid - 1, key);
}

/* ---------- MERGE FUNCTION ---------- */

void merge(int arr[], int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int L[n1];
    int R[n2];

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[low + i];
    }

    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = low;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }

        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* ---------- MERGE SORT ---------- */

void merge_sort(int arr[], int low, int high)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;

        merge_sort(arr, low, mid);
        merge_sort(arr, mid + 1, high);

        merge(arr, low, mid, high);
    }
}

/* ---------- DISPLAY ARRAY ---------- */

void display_array(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

/* ---------- EV NETWORK CONNECTIVITY ANALYSIS ---------- */

void advanced_algorithms_menu(Graph *graph)
{
    if (graph == NULL || graph->node_count == 0)
    {
        printf("No locations available.\n");
        return;
    }

    int arr[MAX_NODES];
    int merge_arr[MAX_NODES];

    int n = graph->node_count;
    int key;
    int result;

    printf("\n====================================\n");
    printf("   EV NETWORK CONNECTIVITY ANALYSIS\n");
    printf("====================================\n");

    /* Extract actual graph connectivity */

    printf("\n===== LOCATION CONNECTIVITY =====\n");

    for (int i = 0; i < n; i++)
    {
        arr[i] = graph->nodes[i].edge_count;
        merge_arr[i] = graph->nodes[i].edge_count;

        printf("%-25s | Road Connections: %d\n",
               graph->nodes[i].name,
               graph->nodes[i].edge_count);
    }

    /* QUICK SORT */

    quick_sort(arr, 0, n - 1);

    printf("\n===== QUICK SORT =====\n");
    printf("Sorted Connectivity (Ascending): ");
    display_array(arr, n);

    /* MERGE SORT */

    merge_sort(merge_arr, 0, n - 1);

    printf("\n===== MERGE SORT =====\n");
    printf("Sorted Connectivity (Ascending): ");
    display_array(merge_arr, n);

    /* BINARY SEARCH */

    printf("\n===== CONNECTIVITY SEARCH =====\n");

    if (!read_integer(
            "Enter number of road connections to search: ",
            &key))
    {
        printf("Invalid input.\n");
        return;
    }

    /* Iterative Binary Search */

    result = binary_search_iterative(arr, n, key);

    if (result != -1)
    {
        printf("\nIterative Binary Search: Value found.\n");

        printf("Locations having %d road connections:\n", key);

        for (int i = 0; i < n; i++)
        {
            if (graph->nodes[i].edge_count == key)
            {
                printf("- %s\n",
                       graph->nodes[i].name);
            }
        }
    }
    else
    {
        printf("\nIterative Binary Search: Value not found.\n");
        printf("No location has %d road connections.\n", key);
    }

    /* Recursive Binary Search */

    result = binary_search_recursive(arr, 0, n - 1, key);

    if (result != -1)
    {
        printf("\nRecursive Binary Search: Value found.\n");
    }
    else
    {
        printf("\nRecursive Binary Search: Value not found.\n");
    }

    printf("\n====================================\n");
}
