#include <stdio.h>
#include <math.h>
#include <omp.h>

int main() {
    int n = 1000000;
    int arr[n];

   
    for (int i = 0; i < n; i++) {
        arr[i] = 1 + (i % 5);
    }

    double log_product_atomic = 0.0;
    double log_product_critical = 0.0;

    double start, end;

    
    start = omp_get_wtime();
    // Atomic Approach 

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        double val = log(arr[i]);

        #pragma omp atomic
        log_product_atomic += val;
    }

    end = omp_get_wtime();
    double time_atomic = end - start;

  // Critical Section Approach 
    start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        double val = log(arr[i]);

        #pragma omp critical
        {
            log_product_critical += val;
        }
    }

    end = omp_get_wtime();
    double time_critical = end - start;


    printf("Log Product (Atomic)   : %f\n", log_product_atomic);
    printf("Time (Atomic)          : %f seconds\n", time_atomic);

    printf("Log Product (Critical) : %f\n", log_product_critical);
    printf("Time (Critical)        : %f seconds\n", time_critical);

    return 0;
}

// Analysis:

// In this program, the product of array elements is computed using two synchronization methods:
// atomic and critical. The atomic directive provides fine-grained synchronization with lower overhead,
//  while the critical directive enforces mutual exclusion on an entire block, increasing waiting time.
  
// For small workloads, critical may appear faster due to reduced contention, but for larger workloads,
// atomic is generally more efficient due to better parallelism.