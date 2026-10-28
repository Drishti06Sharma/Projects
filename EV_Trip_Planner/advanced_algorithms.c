
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


static void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1];
    int R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];
}

void merge_sort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}


/* Test the algorithms */
void advanced_algorithms_menu(void)
{
    int arr[] = {45, 12, 78, 23, 9, 56};
    int n = sizeof(arr) / sizeof(arr[0]);

    quick_sort(arr, 0, n - 1);

        int merge_arr[] = {45, 12, 78, 23, 9, 56};
    int merge_n = (int)(sizeof(merge_arr) / sizeof(merge_arr[0]));

    merge_sort(merge_arr, 0, merge_n - 1);

    printf("\nMerge Sort result: ");
    for (int i = 0; i < merge_n; i++)
        printf("%d ", merge_arr[i]);

    printf("\n");

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
