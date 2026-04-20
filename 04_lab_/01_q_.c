#include <stdio.h>
#include <omp.h>

#define SIZE 600
#define STEP_SIZE 100

int main() {

    int arr[SIZE];
    int sum = 0;

    
    for(int i = 0; i < SIZE; i++) {
        arr[i] = i + 1;   
    }

    printf("\n===== Parallel Array Summation using OpenMP Tasks =====\n");

    
    #pragma omp parallel shared(sum, arr)
    {
        //  Allow multiple threads but only one creates tasks
        #pragma omp single
        {
            printf("Task generator thread: %d\n\n", omp_get_thread_num());

            // Divide array into chunks
            for(int i = 0; i < SIZE; i += STEP_SIZE) {

                int start = i;
                int end = (i + STEP_SIZE < SIZE) ? i + STEP_SIZE : SIZE;

                printf("Thread %d generating task for range [%d - %d]\n",
                       omp_get_thread_num(), start, end - 1);

                //  Create task
                #pragma omp task firstprivate(start, end) shared(sum, arr)
                {
                    int psum = 0;

                    printf("  -> Task executing on thread %d for range [%d - %d]\n",
                           omp_get_thread_num(), start, end - 1);

                    // Compute partial sum
                    for(int j = start; j < end; j++) {
                        psum += arr[j];
                    }

                    // Update global sum safely
                    #pragma omp critical
                    {
                        sum += psum;
                        printf("  -> Thread %d added partial sum %d, global sum now = %d\n",
                               omp_get_thread_num(), psum, sum);
                    }
                }
            }
        }
    }

    printf("\nFinal Global Sum = %d\n", sum);

    return 0;
}

 Analysis:
====================Analysis of Output==================================================

=Parallel execution observed → Multiple threads create and execute tasks.

=Dynamic scheduling → Task creator and executor threads may be different.

=Non-sequential execution → Array ranges may execute in random order.

=Race condition prevented → #pragma omp critical ensures correct global sum.

=Concurrency achieved → Multiple tasks run simultaneously, improving efficiency.

=Conclusion: The program successfully demonstrates task-based parallelism with proper synchronization and correct summation.
