#include <stdio.h>
#include <omp.h>


int parallelFib(int n) {
    int x, y;

    
    if (n <= 1)
        return n;

  
    #pragma omp task shared(x)
    x = parallelFib(n - 1);

   
    #pragma omp task shared(y)
    y = parallelFib(n - 2);

   
    #pragma omp taskwait

    return x + y;
}

int main() {
    int n, result;

    printf("Enter value of n: ");
    scanf("%d", &n);

    double start = omp_get_wtime();

    #pragma omp parallel
    {
       
        #pragma omp single
        {
            result = parallelFib(n);
        }
    }

    double end = omp_get_wtime();

    printf("Fibonacci(%d) = %d\n", n, result);
    printf("Execution time: %f seconds\n", end - start);

    return 0 ;
}

// Analysis of the code:
// 1. The function `parallelFib` computes the Fibonacci number for a given `n` using OpenMP tasks for parallelism.
// 2. The base case for `n <= 1` returns `n` directly, which is correct for Fibonacci numbers.
// 3. For `n > 1`, two tasks are created to compute `parallelFib(n - 1)` and `parallelFib(n - 2)` in parallel.
// 4. The `#pragma omp taskwait` directive ensures that the function waits for both tasks to complete before summing their results and returning the final Fibonacci number.
// 5. The `main` function prompts the user for the value of `n`, measures the execution time, and prints the result along with the execution time.  
