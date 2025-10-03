#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000

void matrix_mult_openmp(double **A, double **B, double **C, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrix_mult_serial(double **A, double **B, double **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

double **allocate_matrix(int n) {
    double **matrix = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }
    return matrix;
}

void free_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void initialize_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double)(i + j);
        }
    }
}

int main() {
    double **A = allocate_matrix(N);
    double **B = allocate_matrix(N);
    double **C = allocate_matrix(N);
    
    initialize_matrix(A, N);
    initialize_matrix(B, N);
    
    printf("Matrix Multiplication Example (%dx%d)\n", N, N);
    printf("Number of threads: %d\n", omp_get_max_threads());
    
    // Serial version
    clock_t start = clock();
    matrix_mult_serial(A, B, C, N);
    clock_t end = clock();
    double serial_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Serial time: %.6f seconds\n", serial_time);
    
    // OpenMP version
    start = clock();
    matrix_mult_openmp(A, B, C, N);
    end = clock();
    double parallel_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("OpenMP time: %.6f seconds\n", parallel_time);
    printf("Speedup: %.2fx\n", serial_time / parallel_time);
    
    // Verify correctness (check a few elements)
    int correct = 1;
    for (int i = 0; i < 10 && correct; i++) {
        for (int j = 0; j < 10; j++) {
            double expected = 0.0;
            for (int k = 0; k < N; k++) {
                expected += A[i][k] * B[k][j];
            }
            if (abs(C[i][j] - expected) > 1e-6) {
                correct = 0;
                break;
            }
        }
    }
    printf("Result correct: %s\n", correct ? "Yes" : "No");
    
    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C, N);
    
    return 0;
}
