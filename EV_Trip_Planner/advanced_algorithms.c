
#include <stdio.h>

/* Quick Sort */
void quick_sort(int arr[], int low, int high)
{
    if (low >= high)
        return;

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

    int p = i + 1;

    quick_sort(arr, low, p - 1);
    quick_sort(arr, p + 1, high);
}

/* Iterative Binary Search */
int binary_search_iterative(int arr[], int n, int key)
{
    int low = 0, high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

/* Recursive Binary Search */
int binary_search_recursive(int arr[], int low, int high, int key)
{
    if (low > high)
        return -1;

    int mid = low + (high - low) / 2;

    if (arr[mid] == key)
        return mid;

    if (arr[mid] < key)
        return binary_search_recursive(arr, mid + 1, high, key);

    return binary_search_recursive(arr, low, mid - 1, key);
}

/* Test the algorithms */
void advanced_algorithms_menu(void)
{
    int arr[] = {45, 12, 78, 23, 9, 56};
    int n = sizeof(arr) / sizeof(arr[0]);

    quick_sort(arr, 0, n - 1);

    printf("\nSorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);

    int key;
    printf("\nEnter number to search: ");
    if (scanf("%d", &key) != 1)
    {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        printf("Invalid input.\n");
        return;
    }

    int result = binary_search_iterative(arr, n, key);

    if (result != -1)
        printf("Iterative Binary Search: Found at index %d\n", result);
    else
        printf("Iterative Binary Search: Not found\n");

    result = binary_search_recursive(arr, 0, n - 1, key);

    if (result != -1)
        printf("Recursive Binary Search: Found at index %d\n", result);
    else
        printf("Recursive Binary Search: Not found\n");
}
