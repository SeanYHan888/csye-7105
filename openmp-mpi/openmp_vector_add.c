#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000

void vector_add_openmp(double *a, double *b, double *c, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

void vector_add_serial(double *a, double *b, double *c, int n) {
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    double *a = (double*)malloc(N * sizeof(double));
    double *b = (double*)malloc(N * sizeof(double));
    double *c = (double*)malloc(N * sizeof(double));
    
    // Initialize vectors
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i * 2;
    }
    
    printf("Vector Addition Example (N = %d)\n", N);
    printf("Number of threads: %d\n", omp_get_max_threads());
    
    // Serial version
    clock_t start = clock();
    vector_add_serial(a, b, c, N);
    clock_t end = clock();
    double serial_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Serial time: %.6f seconds\n", serial_time);
    
    // OpenMP version
    start = clock();
    vector_add_openmp(a, b, c, N);
    end = clock();
    double parallel_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("OpenMP time: %.6f seconds\n", parallel_time);
    printf("Speedup: %.2fx\n", serial_time / parallel_time);
    
    // Verify correctness
    int correct = 1;
    for (int i = 0; i < N; i++) {
        if (c[i] != a[i] + b[i]) {
            correct = 0;
            break;
        }
    }
    printf("Result correct: %s\n", correct ? "Yes" : "No");
    
    free(a);
    free(b);
    free(c);
    
    return 0;
}
