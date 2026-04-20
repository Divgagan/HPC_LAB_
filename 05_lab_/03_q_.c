#include <stdio.h>
#include <omp.h>

#define SIZE 10
int arr[SIZE] = {1,2,3,4,5,6,7,8,9,10};

int parallelSum(int low, int high) {

    if (low == high)
        return arr[low];

    int mid = (low + high) / 2;
    int left=0, right=0;

    #pragma omp task shared(left)
    left = parallelSum(low, mid);

    #pragma omp task shared(right)
    right = parallelSum(mid+1, high);

    #pragma omp taskwait

    printf("Thread %d merged range [%d-%d]\n",
           omp_get_thread_num(), low, high);

    return left + right;
}

int main() {
    int result;

    #pragma omp parallel
    {
        #pragma omp single
        result = parallelSum(0, SIZE-1);
    }

    printf("Sum = %d\n", result);
}

// Analysis of the code:
// 1. The function `parallelSum` computes the sum of elements in the array `
// `arr` between the indices `low` and `high` using a divide-and-conquer approach with OpenMP tasks for parallelism.
// 2. If `low` is equal to `high`, it means we are at
// a single element, and the function returns that element directly.
// 3. For larger ranges, the function calculates the midpoint and creates two tasks to compute the sum of the left and right halves of the range in parallel.
// 4. The `#pragma omp taskwait` directive ensures that the function waits for both tasks to complete before summing their results and returning the total sum for the current range.
// 5. The `main` function initializes the parallel region and calls `parallelSum` for the entire array, then prints the final result.
