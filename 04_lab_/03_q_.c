#include <stdio.h>
#include <omp.h>

int main() {

    omp_set_num_threads(4);

    #pragma omp parallel
    {
        #pragma omp single
        {
            printf("Producer Thread: %d\n\n", omp_get_thread_num());

            for(int i = 0; i < 8; i++) {

                int creator_thread = omp_get_thread_num();

                #pragma omp task firstprivate(i, creator_thread)
                {
                    int executor_thread = omp_get_thread_num();

                    printf("Hello World | Loop index = %d | Created by = %d | Executed by = %d\n",
                           i, creator_thread, executor_thread);
                }
            }

            #pragma omp taskwait
        }
    }

    return 0;
}

// Analysis:
 - One thread (producer) creates tasks.

 - Tasks are executed by any available worker thread.

  - Shows difference between task generation vs execution.

 -  Task creation ≠ task execution.

  - OpenMP runtime schedules tasks dynamically.

   -Load balancing happens automatically.

Thread Behavior: 

Single thread produces tasks.

Multiple threads consume tasks.
