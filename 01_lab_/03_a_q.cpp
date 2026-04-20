#include <iostream>
#include <omp.h>
using namespace std;

int main() {

    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        // Dummy delay loop
        for (long long i = 0; i < 100000000; i++);

        cout << "Hello World from Thread " << id << endl;
    }

    return 0;
}

obervations and Explanations:
The output is non-deterministic due to multiple threads 
accessing shared variables simultaneously, leading to a race condition.

// What this code is doing

// Creates a parallel region using OpenMP

// Multiple threads execute the same code

// A delay loop is added so threads overlap

// Thread ID variable is shared by default

// Why race condition happens

// All threads share the same variable id

// Threads overwrite id simultaneously

// printf / cout is also a shared resource

// Output depends on execution timing

// Observation (Program-1)

// Output order is random

// Thread IDs may repeat or appear incorrect

// Different output on every run

// Output lines appear interleaved / scrambled