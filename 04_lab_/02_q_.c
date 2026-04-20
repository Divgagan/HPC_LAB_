#include <stdio.h>
#include <omp.h>

int main() {
    int x = 10;

    printf("Initial value of x in main: %d\n\n", x);

    #pragma omp parallel
    {
        #pragma omp single
        {
            
            #pragma omp task shared(x)
            {
                x = x + 5;
                printf("SHARED Task -> x = %d, Thread = %d\n", x, omp_get_thread_num());
            }

            
            #pragma omp task private(x)
            {
                x = 100;  
                printf("PRIVATE Task -> x = %d, Thread = %d\n", x, omp_get_thread_num());
            }

            /
            #pragma omp task firstprivate(x)
            {
                x = x + 20;
                printf("FIRSTPRIVATE Task -> x = %d, Thread = %d\n", x, omp_get_thread_num());
            }

            #pragma omp taskwait
        }
    }

    printf("\nFinal value of x in main: %d\n", x);

    return 0;
}


Analysis : 

Shared variable: The shared task modifies the same memory location, so changes affect the final value of x.

Private variable: The private task creates a new independent variable, so its changes do not affect the main variable.

Firstprivate variable: The firstprivate task gets a copy of the parent value at task creation and modifies its local copy only, without changing the main variable.

Conclusion: The output shows how OpenMP handles different variable scopes in tasks and ensures controlled data sharing and isolation.