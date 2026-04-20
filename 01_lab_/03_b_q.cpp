#include <stdio.h>
#include <omp.h>

int main() {
    int id;

    #pragma omp parallel private(id)
    {
        id = omp_get_thread_num();

        for (long long int i = 0; i < 100000000; i++);

        printf("Hello World from Thread %d\n", id);
    }

  
  
    return 0;
}

OBSERVATIONS AND EXPLANATIONS:
The output will consistently show correct thread IDs without duplication or errors.
Using the private() clause ensures each thread has its
 own copy of the variable, eliminating race conditions and producing correct output.
