#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define NUM_STEPS 100000000

double calculate_pi_mpi(int rank, int size) {
    double local_sum = 0.0;
    double step = 1.0 / NUM_STEPS;
    
    // Each process computes its portion
    int start = rank * NUM_STEPS / size;
    int end = (rank + 1) * NUM_STEPS / size;
    
    for (int i = start; i < end; i++) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }
    
    // Sum all local results
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    return global_sum * step;
}

int main(int argc, char *argv[]) {
    int rank, size;
    double start_time, end_time, pi;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        printf("Pi Calculation using Monte Carlo Method\n");
        printf("Number of steps: %d\n", NUM_STEPS);
        printf("Number of processes: %d\n", size);
        start_time = MPI_Wtime();
    }
    
    // Calculate pi
    pi = calculate_pi_mpi(rank, size);
    
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("MPI Pi: %.10f\n", pi);
        printf("MPI time: %.6f seconds\n", end_time - start_time);
        printf("Error: %.2e\n", pi - 3.141592653589793);
    }
    
    MPI_Finalize();
    return 0;
}
