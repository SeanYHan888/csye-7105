#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 1000

void matrix_mult_mpi(double **A, double **B, double **C, int n, int rank, int size) {
    int local_n = n / size;
    int start_row = rank * local_n;
    int end_row = start_row + local_n;
    
    // Each process computes its portion of rows
    for (int i = start_row; i < end_row; i++) {
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

int main(int argc, char *argv[]) {
    int rank, size;
    double **A = NULL, **B = NULL, **C = NULL;
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        printf("Matrix Multiplication Example (%dx%d)\n", N, N);
        printf("Number of processes: %d\n", size);
        
        // Allocate and initialize matrices on root process
        A = allocate_matrix(N);
        B = allocate_matrix(N);
        C = allocate_matrix(N);
        
        initialize_matrix(A, N);
        initialize_matrix(B, N);
    }
    
    // Broadcast matrices to all processes
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    
    // Broadcast matrix A
    for (int i = 0; i < N; i++) {
        MPI_Bcast(A[i], N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    
    // Broadcast matrix B
    for (int i = 0; i < N; i++) {
        MPI_Bcast(B[i], N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    
    // Allocate local matrices for all processes
    if (rank != 0) {
        A = allocate_matrix(N);
        B = allocate_matrix(N);
        C = allocate_matrix(N);
    }
    
    // Perform matrix multiplication
    matrix_mult_mpi(A, B, C, N, rank, size);
    
    // Gather results to root process
    int local_n = N / size;
    for (int i = 0; i < local_n; i++) {
        MPI_Gather(C[rank * local_n + i], N, MPI_DOUBLE,
                   C[rank * local_n + i], N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("MPI time: %.6f seconds\n", end_time - start_time);
        
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
    }
    
    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C, N);
    
    MPI_Finalize();
    return 0;
}
