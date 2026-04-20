#include <stdio.h>
#include <omp.h>

int main() {
    int n = 100;
    int arr[n];

    // Initialization
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    long long global_sum = 0;
    double global_product = 1.0;

    // Parallel Region
    #pragma omp parallel
    {
        long long local_sum = 0;
        double local_product = 1.0;

        // Divide work
        #pragma omp for
        for (int i = 0; i < n; i++) {
            local_sum += arr[i];
            local_product *= arr[i];
        }

        // Named critical for sum
        #pragma omp critical(global_sum_section)
        {
            global_sum += local_sum;
        }

        // Named critical for product
        #pragma omp critical(global_product_section)
        {
            global_product *= local_product;
        }
    }

    // Output
    printf("Global Sum     : %lld\n", global_sum);
    printf("Global Product : %e\n", global_product);

    return 0;
}

// Analysis:
// his program demonstrates the use of multiple named critical sections to 
// update independent shared variables (sum and product). By assigning different names
// to critical sections, separate locks are created, allowing threads to update sum and
// product concurrently without unnecessary blocking. This improves parallel efficiency
// compared to using a single unnamed critical section.