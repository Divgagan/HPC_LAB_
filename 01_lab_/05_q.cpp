#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

int main() {
    const int N = 1000000;
    int arr[N];

    // Sequential initialization
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100000;
    }

    int maxVal = arr[0];

    // here is the parallel region to find the maximum value

    #pragma omp parallel for reduction(max:maxVal)
    for (int i = 0; i < N; i++) {
        if (arr[i] > maxVal)
            maxVal = arr[i];
    }

    cout << "Maximum value: " << maxVal << endl;

    return 0;
}



// Notes  : 


// Reduction ka matlab hota hai:

// multiple threads ke partial results ko safely combine karke ek final result banana

// 2️ Problem without reduction (why it is needed)

// Agar yeh line na hoti:

// #pragma omp parallel for reduction(max:maxVal)


// Aur sirf:

// #pragma omp parallel for


// Toh kya hota?

// Sab threads same maxVal ko update karte

// Multiple threads ek saath write karte

// Race condition

// Galat maximum value

// ⚠️ Maximum calculation parallel me unsafe ho jaata.


////////////////////////
//  ->> 
// The reduction clause allows each thread to compute a private partial result and safely combines them into a single final value, avoiding race conditions.