#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 1000000

void vector_add_mpi(double *a, double *b, double *c, int n, int rank, int size) {
    int local_n = n / size;
    int start = rank * local_n;
    int end = start + local_n;
    
    // Each process computes its portion
    for (int i = start; i < end; i++) {
        c[i] = a[i] + b[i];
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    double *a = NULL, *b = NULL, *c = NULL;
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        printf("Vector Addition Example (N = %d)\n", N);
        printf("Number of processes: %d\n", size);
        
        // Allocate and initialize vectors on root process
        a = (double*)malloc(N * sizeof(double));
        b = (double*)malloc(N * sizeof(double));
        c = (double*)malloc(N * sizeof(double));
        
        for (int i = 0; i < N; i++) {
            a[i] = i;
            b[i] = i * 2;
        }
    }
    
    // Broadcast vectors to all processes
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    
    MPI_Bcast(a, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Allocate local arrays for all processes
    if (rank != 0) {
        a = (double*)malloc(N * sizeof(double));
        b = (double*)malloc(N * sizeof(double));
        c = (double*)malloc(N * sizeof(double));
    }
    
    // Perform vector addition
    vector_add_mpi(a, b, c, N, rank, size);
    
    // Gather results to root process
    MPI_Gather(c + rank * (N/size), N/size, MPI_DOUBLE,
               c, N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("MPI time: %.6f seconds\n", end_time - start_time);
        
        // Verify correctness
        int correct = 1;
        for (int i = 0; i < N; i++) {
            if (c[i] != a[i] + b[i]) {
                correct = 0;
                break;
            }
        }
        printf("Result correct: %s\n", correct ? "Yes" : "No");
    }
    
    free(a);
    free(b);
    free(c);
    
    MPI_Finalize();
    return 0;
}
