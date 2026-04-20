#include <stdio.h>
#include <omp.h>

void swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];

    int i = low - 1;

    int j;

    for(j = low; j < high; j++)
    {
        if(arr[j] < pivot)
        {
            i++;

            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

void quicksort(int arr[], int low, int high)
{
    if(low < high)
    {
        int pi = partition(arr, low, high);

        #pragma omp task
        quicksort(arr, low, pi - 1);

        #pragma omp task
        quicksort(arr, pi + 1, high);

        #pragma omp taskwait
    }
}

int main()
{
    int arr[12] = {12, 5, 8, 19, 3, 15, 7, 1, 10, 6, 2, 11};

    int i;

    printf("Original Array:\n");

    for(i = 0; i < 12; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    #pragma omp parallel
    {
        #pragma omp single
        {
            quicksort(arr, 0, 11);
        }
    }

    printf("\nSorted Array:\n");

    for(i = 0; i < 12; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}


// Analysis of the code:
// “In this program, QuickSort is implemented using OpenMP task-based parallelism. 
// The array is divided into subarrays using the partition function, and each subarray is sorted recursively.

// Using #pragma omp task, the left and right subarrays are processed in parallel. 
// The taskwait directive ensures synchronization between recursive calls.

// This approach improves performance by utilizing multiple threads to handle independent subproblems simultaneously.”