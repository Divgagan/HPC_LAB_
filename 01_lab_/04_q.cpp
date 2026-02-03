#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int N = 10000000;

    int *A = new int[N];
    int *B = new int[N];
    int *C = new int[N];

    // Initialization
    for (int i = 0; i < N; i++) {
        A[i] = i;
        B[i] = 2 * i;
    }

    /* ---------- Sequential Execution ---------- */
    double start_seq = omp_get_wtime();

    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    double end_seq = omp_get_wtime();

    cout << "Sequential execution time: "
         << end_seq - start_seq << " seconds" << endl;

    /* ---------- Parallel Execution ---------- */
    double start_par = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    double end_par = omp_get_wtime();

    cout << "Parallel execution time: "
         << end_par - start_par << " seconds" << endl;

    // Free memory
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
