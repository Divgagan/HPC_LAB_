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

