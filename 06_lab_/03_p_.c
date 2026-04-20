#include <stdio.h>
#include <omp.h>

#define N 100
#define BUCKETS 4

int main() {
    int arr[N];
    for (int i = 0; i < N; i++) arr[i] = i + 1;

    // ---------- Global Lock ----------
    omp_lock_t lock;
    omp_init_lock(&lock);

    long long sum_global = 0;
    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        omp_set_lock(&lock);
        sum_global += arr[i];
        omp_unset_lock(&lock);
    }

    double time_global = omp_get_wtime() - start;
    omp_destroy_lock(&lock);

    // ---------- Bucket Lock ----------
    omp_lock_t bucket_locks[BUCKETS];
    long long bucket_sum[BUCKETS] = {0};

    for (int i = 0; i < BUCKETS; i++)
        omp_init_lock(&bucket_locks[i]);

    start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int b = i % BUCKETS;

        omp_set_lock(&bucket_locks[b]);
        bucket_sum[b] += arr[i];
        omp_unset_lock(&bucket_locks[b]);
    }

    long long sum_bucket = 0;
    for (int i = 0; i < BUCKETS; i++) {
        sum_bucket += bucket_sum[i];
        omp_destroy_lock(&bucket_locks[i]);
    }

    double time_bucket = omp_get_wtime() - start;

    printf("Global Lock Sum   : %lld, Time: %f\n", sum_global, time_global);
    printf("Bucket Lock Sum   : %lld, Time: %f\n", sum_bucket, time_bucket);

    return 0;
}

// Analysis:

// Global lock causes high contention as all threads compete for a single lock, 
// reducing parallelism. Bucket locking reduces contention by allowing multiple threads
// to work on different partitions simultaneously. However, managing multiple locks 
// introduces slight overhead. Overall, bucket locking improves performance for larger workloads.