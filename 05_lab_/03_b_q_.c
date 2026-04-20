#include <stdio.h>
#include <omp.h>

#define SIZE 10
int arr[SIZE];

int main() {

    int sum = 0;

    #pragma omp parallel
    {
        
        #pragma omp for nowait
        for(int i=0;i<SIZE;i++)
            arr[i] = i+1;

        
        #pragma omp taskwait

       
        #pragma omp for reduction(+:sum)
        for(int i=0;i<SIZE;i++)
            sum += arr[i];
    }

    printf("Sum = %d\n", sum);
}

// Analysis of the code:
// 1. The code initializes an array `arr` of size `SIZE` with values
// from 1 to `SIZE` in parallel using an OpenMP `for` loop with the `nowait` clause to allow the next section to start without waiting for the initialization to complete.
// 2. The `#pragma omp taskwait` directive ensures that all tasks created in
    // the previous section are completed before proceeding to the next section, which computes the sum of the elements in the array.
// 3. The second `for` loop uses the `reduction` clause to safely
    // compute the sum of the elements in parallel, ensuring that the updates to `sum` are done atomically to avoid race conditions
 // 4. Finally, the computed sum is printed to the console.
 // 5. The `main` function demonstrates the use of OpenMP for parallelizing both the initialization of the array 
 //  and the computation of the sum, showcasing how to manage dependencies between tasks using `taskwait`.