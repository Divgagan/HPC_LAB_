#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    int n;

    cout << "Enter number of threads: ";
    cin >> n;

    #pragma omp parallel num_threads(n)
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        
        
        for (int i = 0; i < total; i++) {
            #pragma omp barrier
            if (i == id) {
                cout << "Hello World from Thread "
                     << id << " of " << total << endl;
            }
        }
    }

    return 0;
}
