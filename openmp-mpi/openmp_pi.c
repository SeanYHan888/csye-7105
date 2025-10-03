#include <stdio.h>
#include <omp.h>
#include <time.h>

#define NUM_STEPS 100000000

double calculate_pi_openmp() {
    double pi = 0.0;
    double step = 1.0 / NUM_STEPS;
    
    #pragma omp parallel
    {
        double local_sum = 0.0;
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        
        // Each thread processes a portion of the range
        int start = thread_id * NUM_STEPS / num_threads;
        int end = (thread_id + 1) * NUM_STEPS / num_threads;
        
        for (int i = start; i < end; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        
        #pragma omp atomic
        pi += local_sum;
    }
    
    return pi * step;
}

double calculate_pi_serial() {
    double pi = 0.0;
    double step = 1.0 / NUM_STEPS;
    
    for (int i = 0; i < NUM_STEPS; i++) {
        double x = (i + 0.5) * step;
        pi += 4.0 / (1.0 + x * x);
    }
    
    return pi * step;
}

int main() {
    printf("Pi Calculation using Monte Carlo Method\n");
    printf("Number of steps: %d\n", NUM_STEPS);
    printf("Number of threads: %d\n", omp_get_max_threads());
    
    // Serial version
    clock_t start = clock();
    double pi_serial = calculate_pi_serial();
    clock_t end = clock();
    double serial_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Serial Pi: %.10f\n", pi_serial);
    printf("Serial time: %.6f seconds\n", serial_time);
    
    // OpenMP version
    start = clock();
    double pi_parallel = calculate_pi_openmp();
    end = clock();
    double parallel_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("OpenMP Pi: %.10f\n", pi_parallel);
    printf("OpenMP time: %.6f seconds\n", parallel_time);
    printf("Speedup: %.2fx\n", serial_time / parallel_time);
    
    printf("Error: %.2e\n", pi_parallel - 3.141592653589793);
    
    return 0;
}
