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

/* ---------- ADVANCED ALGORITHMS MENU ---------- */

void advanced_algorithms_menu(void)
{
    int arr[] = {45, 12, 78, 23, 9, 56};
    int merge_arr[] = {45, 12, 78, 23, 9, 56};

    int n = sizeof(arr) / sizeof(arr[0]);
    int key;
    int result;

    printf("\n====================================\n");
    printf("       ADVANCED ALGORITHMS\n");
    printf("====================================\n");

    /* Quick Sort */

    printf("\nOriginal Array:\n");
    display_array(arr, n);

    quick_sort(arr, 0, n - 1);

    printf("\nArray after Quick Sort:\n");
    display_array(arr, n);

    /* Merge Sort */

    printf("\nOriginal Array for Merge Sort:\n");
    display_array(merge_arr, n);

    merge_sort(merge_arr, 0, n - 1);

    printf("\nArray after Merge Sort:\n");
    display_array(merge_arr, n);

    /* Binary Search */

    printf("\n===== BINARY SEARCH =====\n");

    printf("Sorted array: ");
    display_array(arr, n);

    if (!read_integer("Enter the number to search: ", &key))
    {
        printf("Invalid input. Please enter an integer.\n");
        return;
    }

    /* Iterative Binary Search */

    result = binary_search_iterative(arr, n, key);

    if (result != -1)
    {
        printf("Iterative Binary Search: "
               "Element found at index %d\n", result);
    }
    else
    {
        printf("Iterative Binary Search: Element not found\n");
    }

    /* Recursive Binary Search */

    result = binary_search_recursive(arr, 0, n - 1, key);

    if (result != -1)
    {
        printf("Recursive Binary Search: "
               "Element found at index %d\n", result);
    }
    else
    {
        printf("Recursive Binary Search: Element not found\n");
    }

    printf("\n====================================\n");
}