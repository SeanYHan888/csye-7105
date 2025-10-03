# OpenMP vs MPI: Complete Parallel Programming Learning Suite

This repository provides a comprehensive learning resource for parallel programming using both **OpenMP** (shared memory) and **MPI** (distributed memory) paradigms in C. Each algorithm is implemented using both approaches with detailed explanations, performance analysis, and practical examples.

## 📁 Complete File Structure

### OpenMP Examples (Shared Memory Parallelism)

- **`openmp_vector_add.c`** - Vector addition with performance benchmarking
- **`openmp_matrix_mult.c`** - Matrix multiplication with timing analysis
- **`openmp_pi.c`** - Pi calculation using numerical integration with OpenMP

### MPI Examples (Distributed Memory Parallelism)

- **`mpi_vector_add.c`** - Vector addition using message passing
- **`mpi_matrix_mult.c`** - Matrix multiplication with process distribution
- **`mpi_pi.c`** - Pi calculation with distributed computation

### Documentation & Build System

- **`COMPARISON.md`** - Detailed technical comparison and analysis
- **`Makefile`** - Complete build system with multiple targets
- **`README.md`** - This comprehensive guide

## 🎯 Learning Objectives

By working through these examples, you will learn:

1. **OpenMP Fundamentals**

   - Directive-based parallel programming
   - Thread management and synchronization
   - Shared memory programming concepts
   - Performance optimization techniques

2. **MPI Fundamentals**

   - Message passing interface concepts
   - Process communication patterns
   - Distributed memory programming
   - Collective operations and data distribution

3. **Performance Analysis**

   - Speedup and efficiency calculations
   - Scalability considerations
   - Memory bandwidth analysis
   - Load balancing strategies

4. **Algorithm Implementation**
   - Vector operations (embarrassingly parallel)
   - Matrix operations (data decomposition)
   - Numerical integration (work distribution)

## 🚀 Quick Start Guide

### Prerequisites

**System Requirements:**

- C compiler (GCC 4.0+ recommended)
- OpenMP support (included in most modern GCC)
- MPI implementation (OpenMPI, MPICH, or Intel MPI)

**Installation Commands:**

```bash
# macOS (using Homebrew)
brew install gcc open-mpi

# Ubuntu/Debian
sudo apt-get update
sudo apt-get install gcc libopenmpi-dev

# CentOS/RHEL
sudo yum install gcc openmpi-devel

# Verify installations
gcc --version
mpicc --version
```

### One-Command Setup

```bash
# Clone and build everything
git clone <repository-url>
cd csye-7105
make all

# Run performance comparison
make compare
```

## 📊 Detailed Code Analysis

### 1. Vector Addition Examples

#### OpenMP Implementation (`openmp_vector_add.c`)

**Key Features:**

- **Algorithm**: Element-wise vector addition `c[i] = a[i] + b[i]`
- **Parallelization**: `#pragma omp parallel for` directive
- **Problem Size**: 1,000,000 elements (configurable)
- **Performance Metrics**: Serial vs parallel timing, speedup calculation

**Code Highlights:**

```c
// Parallel vector addition
void vector_add_openmp(double *a, double *b, double *c, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}
```

**Learning Points:**

- Embarrassingly parallel problem (no dependencies)
- Automatic load balancing by OpenMP
- Memory access pattern optimization
- False sharing considerations

#### MPI Implementation (`mpi_vector_add.c`)

**Key Features:**

- **Algorithm**: Same vector addition with distributed data
- **Parallelization**: Process-based with message passing
- **Data Distribution**: Each process handles `N/size` elements
- **Communication**: `MPI_Bcast` for data distribution, `MPI_Gather` for results

**Code Highlights:**

```c
// Distribute work among processes
int local_n = n / size;
int start = rank * local_n;
int end = start + local_n;

// Each process computes its portion
for (int i = start; i < end; i++) {
    c[i] = a[i] + b[i];
}
```

**Learning Points:**

- Data decomposition strategies
- Collective communication patterns
- Process synchronization
- Memory management across processes

### 2. Matrix Multiplication Examples

#### OpenMP Implementation (`openmp_matrix_mult.c`)

**Key Features:**

- **Algorithm**: Standard matrix multiplication `C = A × B`
- **Parallelization**: Row-wise parallelization with `#pragma omp parallel for`
- **Problem Size**: 1000×1000 matrices (configurable)
- **Memory Management**: Dynamic allocation with proper cleanup

**Code Highlights:**

```c
// Parallel matrix multiplication
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
```

**Learning Points:**

- Cache-friendly memory access patterns
- Loop optimization techniques
- Memory bandwidth considerations
- Load balancing in nested loops

#### MPI Implementation (`mpi_matrix_mult.c`)

**Key Features:**

- **Algorithm**: Same matrix multiplication with distributed computation
- **Parallelization**: Row-wise distribution across processes
- **Communication**: Broadcast matrices, gather results
- **Scalability**: Handles large matrices across multiple processes

**Code Highlights:**

```c
// Distribute rows among processes
int local_n = n / size;
int start_row = rank * local_n;
int end_row = start_row + local_n;

// Each process computes its row portion
for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < n; j++) {
        // Compute C[i][j]
    }
}
```

**Learning Points:**

- 2D data decomposition
- Broadcast vs point-to-point communication
- Load balancing strategies
- Memory scalability

### 3. Pi Calculation Examples

#### OpenMP Implementation (`openmp_pi.c`)

**Key Features:**

- **Algorithm**: Numerical integration using trapezoidal rule
- **Formula**: π = ∫₀¹ 4/(1+x²) dx ≈ (1/N) × Σᵢ₌₀ᴺ⁻¹ 4/(1+((i+0.5)/N)²)
- **Parallelization**: Work distribution with atomic operations
- **Problem Size**: 100,000,000 integration steps (configurable)

**Code Highlights:**

```c
// Parallel pi calculation
double calculate_pi_openmp() {
    double pi = 0.0;
    double step = 1.0 / NUM_STEPS;

    #pragma omp parallel
    {
        double local_sum = 0.0;
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Each thread processes a portion
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
```

**Learning Points:**

- Reduction operations
- Atomic operations for thread safety
- Work distribution strategies
- Numerical precision considerations

#### MPI Implementation (`mpi_pi.c`)

**Key Features:**

- **Algorithm**: Same numerical integration with distributed computation
- **Parallelization**: Process-based work distribution
- **Communication**: `MPI_Reduce` for global sum
- **Efficiency**: Minimal communication overhead

**Code Highlights:**

```c
// Distributed pi calculation
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
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);

    return global_sum * step;
}
```

**Learning Points:**

- Reduction operations in MPI
- Work distribution strategies
- Communication efficiency
- Numerical accuracy in distributed computing

## 🔧 Complete Build System

### Makefile Targets

```bash
# Build all examples
make all

# Build specific technology
make openmp          # Build all OpenMP examples
make mpi             # Build all MPI examples

# Run examples
make run-openmp      # Run all OpenMP examples
make run-mpi         # Run MPI with 4 processes
make run-mpi-2       # Run MPI with 2 processes
make compare         # Run performance comparison

# Utilities
make clean           # Remove all executables
make help            # Show all available targets
```

### Manual Compilation

#### OpenMP Compilation

```bash
# Basic compilation
gcc -fopenmp -O2 openmp_vector_add.c -o openmp_vector_add

# With optimization flags
gcc -fopenmp -O3 -march=native -mtune=native openmp_vector_add.c -o openmp_vector_add

# Debug version
gcc -g -fopenmp -O0 openmp_vector_add.c -o openmp_vector_add_debug
```

#### MPI Compilation

```bash
# Basic compilation
mpicc -O2 mpi_vector_add.c -o mpi_vector_add

# With optimization flags
mpicc -O3 -march=native -mtune=native mpi_vector_add.c -o mpi_vector_add

# Debug version
mpicc -g -O0 mpi_vector_add.c -o mpi_vector_add_debug
```

### Running Examples

#### OpenMP Execution

```bash
# Default (uses all available cores)
./openmp_vector_add

# Specify number of threads
OMP_NUM_THREADS=4 ./openmp_vector_add

# Set thread affinity (Linux)
OMP_NUM_THREADS=4 OMP_PROC_BIND=true ./openmp_vector_add
```

#### MPI Execution

```bash
# Run with specific number of processes
mpirun -np 4 ./mpi_vector_add

# Run on specific hosts
mpirun -np 4 -hostfile hosts.txt ./mpi_vector_add

# Run with process binding
mpirun -np 4 --bind-to core ./mpi_vector_add
```

## 📈 Performance Analysis

### Expected Performance Results

#### Vector Addition (1M elements)

```
OpenMP (8 threads):
- Serial time: 0.002450 seconds
- Parallel time: 0.000450 seconds
- Speedup: 5.44x
- Efficiency: 68%

MPI (4 processes):
- Parallel time: 0.000800 seconds
- Speedup: 3.06x
- Efficiency: 77%
```

#### Matrix Multiplication (1000×1000)

```
OpenMP (8 threads):
- Serial time: 2.345000 seconds
- Parallel time: 0.312000 seconds
- Speedup: 7.52x
- Efficiency: 94%

MPI (4 processes):
- Parallel time: 0.650000 seconds
- Speedup: 3.61x
- Efficiency: 90%
```

#### Pi Calculation (100M steps)

```
OpenMP (8 threads):
- Serial time: 0.450000 seconds
- Parallel time: 0.062000 seconds
- Speedup: 7.26x
- Efficiency: 91%

MPI (4 processes):
- Parallel time: 0.120000 seconds
- Speedup: 3.75x
- Efficiency: 94%
```

### Performance Optimization Tips

#### OpenMP Optimization

```c
// Use appropriate scheduling
#pragma omp parallel for schedule(static)
#pragma omp parallel for schedule(dynamic, 100)
#pragma omp parallel for schedule(guided)

// Minimize false sharing
#pragma omp parallel for schedule(static, 1)

// Use vectorization
#pragma omp simd
for (int i = 0; i < n; i++) {
    c[i] = a[i] + b[i];
}
```

#### MPI Optimization

```c
// Use non-blocking communication
MPI_Isend(buffer, count, MPI_DOUBLE, dest, tag, comm, &request);
MPI_Irecv(buffer, count, MPI_DOUBLE, source, tag, comm, &request);

// Overlap computation and communication
MPI_Isend(...);
// Do computation
MPI_Wait(&request, &status);

// Use derived datatypes for complex data
MPI_Type_vector(rows, cols, stride, MPI_DOUBLE, &vector_type);
```

## 🛠️ Advanced Configuration

### Environment Variables

#### OpenMP Configuration

```bash
# Number of threads
export OMP_NUM_THREADS=8

# Thread binding
export OMP_PROC_BIND=true
export OMP_PLACES=cores

# Scheduling
export OMP_SCHEDULE=dynamic,100

# Debugging
export OMP_DISPLAY_ENV=true
```

#### MPI Configuration

```bash
# Process binding
export OMPI_MCA_hwloc_base_binding_policy=core

# Communication protocols
export OMPI_MCA_btl=^openib

# Debugging
export OMPI_MCA_verbose=1
```

### Customization Options

#### Problem Size Configuration

```c
// In each source file, modify these constants:
#define N 1000000          // Vector/matrix size
#define NUM_STEPS 100000000 // Pi calculation steps

// Or use command line arguments:
int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 1000000;
    // ... rest of the code
}
```

#### Compiler Optimization

```makefile
# In Makefile, modify CFLAGS:
CFLAGS = -O3 -march=native -mtune=native -funroll-loops
CFLAGS += -ffast-math -fopenmp-simd
```

## 🐛 Troubleshooting Guide

### Common Issues and Solutions

#### OpenMP Issues

**Problem**: OpenMP not working

```bash
# Check OpenMP support
gcc -fopenmp -dM -E - < /dev/null | grep -i openmp

# Solution: Install GCC with OpenMP
brew install gcc  # macOS
sudo apt-get install gcc  # Ubuntu
```

**Problem**: Poor performance

```bash
# Check thread count
echo $OMP_NUM_THREADS

# Check CPU info
lscpu  # Linux
sysctl -n hw.ncpu  # macOS
```

#### MPI Issues

**Problem**: MPI not found

```bash
# Check MPI installation
which mpicc
which mpirun

# Solution: Install MPI
brew install open-mpi  # macOS
sudo apt-get install libopenmpi-dev  # Ubuntu
```

**Problem**: Permission denied

```bash
# Check SSH keys for remote execution
ssh-keygen -t rsa
ssh-copy-id user@hostname
```

**Problem**: Process binding issues

```bash
# Run with explicit binding
mpirun -np 4 --bind-to core ./mpi_vector_add

# Or disable binding
mpirun -np 4 --bind-to none ./mpi_vector_add
```

### Debugging Tools

#### OpenMP Debugging

```bash
# Compile with debug info
gcc -g -fopenmp -O0 openmp_vector_add.c -o debug_openmp

# Use GDB
gdb ./debug_openmp
(gdb) set environment OMP_NUM_THREADS=2
(gdb) run

# Use Valgrind for memory issues
valgrind --tool=memcheck ./debug_openmp
```

#### MPI Debugging

```bash
# Compile with debug info
mpicc -g -O0 mpi_vector_add.c -o debug_mpi

# Use GDB with MPI
mpirun -np 2 xterm -e gdb ./debug_mpi

# Use Valgrind with MPI
mpirun -np 2 valgrind --tool=memcheck ./debug_mpi
```

## 📚 Educational Resources

### Recommended Reading

1. **OpenMP Resources**

   - [OpenMP 5.0 Specification](https://www.openmp.org/specifications/)
   - [OpenMP Tutorial](https://www.openmp.org/resources/tutorials-articles/)
   - [Intel OpenMP Documentation](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/oneapi-compiler.html)

2. **MPI Resources**

   - [MPI 4.0 Standard](https://www.mpi-forum.org/docs/mpi-4.0/mpi40-report.pdf)
   - [MPI Tutorial](https://mpitutorial.com/)
   - [OpenMPI Documentation](https://www.open-mpi.org/doc/)

3. **Parallel Programming Concepts**
   - "Introduction to Parallel Computing" by Ananth Grama
   - "Patterns for Parallel Programming" by Mattson, Sanders, and Massingill
   - "High Performance Computing" by Dowd and Severance

### Online Courses

- [Coursera: Parallel Programming](https://www.coursera.org/learn/parallel-programming)
- [edX: Introduction to High Performance Computing](https://www.edx.org/course/introduction-to-high-performance-computing)
- [MIT OpenCourseWare: Parallel Computing](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/)

## 🤝 Contributing

We welcome contributions to improve these examples:

1. **Bug Reports**: Report issues with specific examples
2. **Performance Improvements**: Submit optimized versions
3. **New Examples**: Add more parallel algorithms
4. **Documentation**: Improve explanations and comments
5. **Testing**: Add unit tests and validation

### Contribution Guidelines

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is open source and available under the MIT License. See the LICENSE file for details.

## 🙏 Acknowledgments

- OpenMP Architecture Review Board for the OpenMP standard
- MPI Forum for the MPI standard
- The parallel computing community for best practices
- Contributors and users who provide feedback

---

**Happy Parallel Programming! 🚀**

For questions or support, please open an issue in the repository.
