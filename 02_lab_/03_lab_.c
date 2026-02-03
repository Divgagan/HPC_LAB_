#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Thread %d reached point A\n", tid);

        #pragma omp barrier

        printf("Thread %d passed the barrier\n", tid);
    }
    return 0;
}

OBservations: 


- Multiple threads run in parallel and print a message when they reach point A.

- Each thread then waits at the barrier until all other threads arrive.

- The barrier ensures that no thread moves ahead before synchronization.

- After synchronization, all threads continue and print the second message.



















/*🔑 One-line memory trick (very useful)

barrier → “Sab ruk jao”

single → “Ek kare, baaki wait”

single nowait → “Ek kare, baaki chalte raho”

master → “Sirf boss kare, koi wait nahi”*/