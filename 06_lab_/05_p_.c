#include <stdio.h>
#include <omp.h>

#define N 400

int main() {
    int A[N], B[N];

    
    for (int i = 0; i < N; i++) {
        A[i] = i + 1;
        B[i] = (i % 10) + 1;   
    }

    long long sum = 0, diff = 0;
    double product = 1.0, division = 0.0;

    #pragma omp parallel
    {
        #pragma omp single
        {
            
            #pragma omp task
            {
                long long local_sum = 0;
                for (int i = 0; i < N; i++)
                    local_sum += A[i] + B[i];

                #pragma omp critical
                sum += local_sum;
            }

            
            #pragma omp task
            {
                long long local_diff = 0;
                for (int i = 0; i < N; i++)
                    local_diff += (A[i] - B[i]);

                #pragma omp critical
                diff += local_diff;
            }

            /
            #pragma omp task
            {
                double local_product = 1.0;
                for (int i = 0; i < 20; i++)   
                    local_product *= (A[i] * B[i]);

                #pragma omp critical
                product *= local_product;
            }

           
            #pragma omp task
            {
                double local_div = 0.0;
                for (int i = 0; i < N; i++)
                    local_div += (double)A[i] / B[i];

                #pragma omp critical
                division += local_div;
            }

           
            #pragma omp taskwait

           
            printf("Sum        : %lld\n", sum);
            printf("Difference : %lld\n", diff);
            printf("Product    : %e\n", product);
            printf("Division   : %f\n", division);
        }
    }

    return 0;
}

Analysis:
The program uses multiple tasks to perform independent 
computations in parallel. Race conditions are avoided using 
local variables and critical sections. Task synchronization is
ensured using taskwait, guaranteeing that results are printed
only after all tasks complete.