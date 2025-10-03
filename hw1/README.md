# CSYE7105 Homework 1 - Parallel Computing Exercises

This README explains the completed homework exercises covering OpenMP and MPI parallel programming concepts.

## Table of Contents

- [Exercise 1: OpenMP Environment Information](#exercise-1-openmp-environment-information)
- [Exercise 2: OpenMP Vector Addition](#exercise-2-openmp-vector-addition)
- [Exercise 3: OpenMP Matrix Multiplication](#exercise-3-openmp-matrix-multiplication)
- [Exercise 4: MPI Hello World](#exercise-4-mpi-hello-world)
- [Compilation Instructions](#compilation-instructions)

---

## Exercise 1: OpenMP Environment Information

**File:** `csye7105_ex1.c`  
**Points:** 14 total

### What This Exercise Teaches

This exercise demonstrates how to query OpenMP runtime environment information, which is crucial for understanding your parallel execution environment.

### Key Concepts

#### 1. Thread Identification

```c
tid = omp_get_thread_num();
```

- **Purpose:** Gets the unique thread ID (0 to num_threads-1)
- **Usage:** Each thread gets a different ID to identify itself
- **Points:** 2

#### 2. Environment Queries

The master thread (tid == 0) queries various OpenMP environment settings:

```c
procs = omp_get_num_procs();        // Number of processors available
nthreads = omp_get_num_threads();   // Number of threads in current team
maxt = omp_get_max_threads();       // Maximum threads available
inpar = omp_in_parallel();          // Is currently in parallel region?
dynamic = omp_get_dynamic();        // Dynamic thread adjustment enabled?
nested = omp_get_nested();          // Nested parallelism supported?
```

**Points:** 2 points each (12 total)

### Why This Matters

Understanding your parallel environment helps you:

- Debug parallel programs
- Optimize thread usage
- Make informed decisions about parallelization strategies

---

## Exercise 2: OpenMP Vector Addition

**File:** `csye7105_ex2.c`  
**Points:** 14 total

### What This Exercise Teaches

This exercise demonstrates basic OpenMP parallelization with proper data scoping and thread management.

### Key Concepts

#### 1. Parallel Directive

```c
#pragma omp parallel num_threads(N_THREADS) private(tid, i) shared(a, b, c, chunk)
```

- **`#pragma omp parallel`:** Creates a parallel region
- **`num_threads(N_THREADS)`:** Explicitly sets number of threads (2 points)
- **`private(tid, i)`:** Each thread gets its own copy of these variables (6 points)
- **`shared(a, b, c, chunk)`:** All threads share these variables
- **Points:** 5 for parallelization + 6 for data scope + 2 for num_threads + 1 for placement = 14

#### 2. Work Distribution

```c
for (i = tid * chunk; i < (tid+1) * chunk; i++) {
    c[i] = a[i] + b[i];
}
```

- Each thread processes a different chunk of the array
- `chunk = N/N_THREADS` divides work evenly
- Thread 0: elements 0 to chunk-1
- Thread 1: elements chunk to 2\*chunk-1
- And so on...

#### 3. Array Initialization

```c
for (i = 0; i < N; i++) {
    a[i] = i * 1.0;
    b[i] = i * 2.0;
}
```

- Added to make the program complete and testable
- `c[i] = a[i] + b[i]` will result in `c[i] = 3*i`

### Why This Matters

- **Data Scoping:** Critical for avoiding race conditions
- **Work Distribution:** Shows how to divide work among threads
- **Thread Management:** Demonstrates explicit thread control

---

## Exercise 3: OpenMP Matrix Multiplication

**File:** `csye7105_ex3.c`  
**Points:** 14 total

### What This Exercise Teaches

This exercise demonstrates parallel matrix multiplication with OpenMP scheduling and proper data scoping.

### Key Concepts

#### 1. Parallel Region with Data Scoping

```c
#pragma omp parallel private(tid) shared(nthreads, a, b, c)
```

- **`private(tid)`:** Each thread has its own thread ID
- **`shared(nthreads, a, b, c)`:** All threads share these variables
- **Points:** 10

#### 2. Static Scheduling

```c
#pragma omp for schedule(static, chunk)
for (i=0; i<NRA; i++) {
    // Matrix multiplication loops
}
```

- **`schedule(static, chunk)`:** Distributes iterations in chunks statically
- **Static:** Each thread gets fixed chunks assigned at the beginning
- **Chunk size:** 10 iterations per chunk
- **Points:** 4

#### 3. Matrix Multiplication Algorithm

```c
for (i=0; i<NRA; i++) {
    for(j=0; j<NCB; j++) {
        for (k=0; k<NCA; k++) {
            c[i][j] += a[i][k] * b[k][j];
        }
    }
}
```

- Standard O(n³) matrix multiplication
- Only the outer loop is parallelized
- Inner loops remain sequential for each thread

### Why This Matters

- **Scheduling:** Different scheduling strategies affect performance
- **Load Balancing:** Static scheduling works well for uniform work
- **Nested Loops:** Shows how to parallelize outer loops while keeping inner loops sequential

---

## Exercise 4: MPI Hello World

**File:** `csye7105_ex4.c`  
**Points:** 8 total

### What This Exercise Teaches

This exercise demonstrates basic MPI (Message Passing Interface) program structure and process management.

### Key Concepts

#### 1. MPI Initialization

```c
MPI_Init(&argc, &argv);
```

- **Purpose:** Initializes the MPI environment
- **Parameters:** Command line arguments (passed through)
- **Must be called:** Before any other MPI function

#### 2. Process Information

```c
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);  // Get total number of processes
MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // Get current process rank
```

- **`MPI_COMM_WORLD`:** Default communicator (all processes)
- **`numtasks`:** Total number of MPI processes
- **`rank`:** Unique process ID (0 to numtasks-1)
- **Points:** 2 each (4 total)

#### 3. Process Name

```c
MPI_Get_processor_name(hostname, &len);
```

- Gets the name of the processor/node running this process
- Useful for debugging and understanding process distribution

#### 4. MPI Cleanup

```c
MPI_Finalize();
```

- **Purpose:** Cleans up MPI environment
- **Must be called:** Before program termination
- **Points:** 2

### Why This Matters

- **Process Management:** Understanding how MPI manages multiple processes
- **Communication:** Foundation for inter-process communication
- **Distributed Computing:** Core concepts for cluster computing

---

## Compilation Instructions

### For OpenMP Programs (Exercises 1-3)

```bash
# Compile with OpenMP support
gcc -fopenmp -o ex1 csye7105_ex1.c
gcc -fopenmp -o ex2 csye7105_ex2.c
gcc -fopenmp -o ex3 csye7105_ex3.c

# Run the programs
./ex1
./ex2
./ex3
```

### For MPI Program (Exercise 4)

```bash
# Compile with MPI support
mpicc -o ex4 csye7105_ex4.c

# Run with multiple processes
mpirun -np 4 ./ex4
```

### Alternative Compilation

```bash
# Using make (if Makefile exists)
make

# Or with specific flags
gcc -fopenmp -O2 -Wall -o ex1 csye7105_ex1.c
mpicc -O2 -Wall -o ex4 csye7105_ex4.c
```

---

## Key Learning Outcomes

After completing these exercises, you should understand:

1. **OpenMP Basics:**

   - How to create parallel regions
   - Data scoping (private vs shared)
   - Thread management and environment queries
   - Work distribution strategies

2. **MPI Basics:**

   - Process initialization and cleanup
   - Process identification and communication
   - Distributed computing concepts

3. **Parallel Programming Concepts:**

   - Race conditions and data scoping
   - Work distribution and load balancing
   - Performance considerations

4. **Practical Skills:**
   - Debugging parallel programs
   - Choosing appropriate parallelization strategies
   - Understanding execution environments

---

## Common Pitfalls to Avoid

1. **Data Scoping Errors:**

   - Forgetting to declare variables as private
   - Accidentally sharing variables that should be private

2. **Race Conditions:**

   - Multiple threads accessing shared data without synchronization
   - Incorrect work distribution

3. **MPI Errors:**

   - Forgetting to call `MPI_Init()` or `MPI_Finalize()`
   - Using MPI functions before initialization

4. **Performance Issues:**
   - Poor work distribution
   - Inappropriate scheduling strategies
   - Excessive synchronization

---

## Next Steps

To further your understanding:

1. **Experiment with different thread counts** in OpenMP programs
2. **Try different scheduling strategies** (dynamic, guided, auto)
3. **Implement more complex MPI programs** with communication
4. **Profile your programs** to understand performance characteristics
5. **Study load balancing** and optimization techniques

Happy parallel programming! 🚀
