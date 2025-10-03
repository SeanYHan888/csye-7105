# Makefile for OpenMP and MPI Examples
# Compiler settings
CC = gcc
MPICC = mpicc
CFLAGS = -O2 -Wall -Wextra
OPENMP_FLAGS = -fopenmp

# Target executables
OPENMP_TARGETS = openmp_vector_add openmp_matrix_mult openmp_pi
MPI_TARGETS = mpi_vector_add mpi_matrix_mult mpi_pi

# Default target
all: openmp mpi

# OpenMP targets
openmp: $(OPENMP_TARGETS)

openmp_vector_add: openmp_vector_add.c
	$(CC) $(CFLAGS) $(OPENMP_FLAGS) -o $@ $<

openmp_matrix_mult: openmp_matrix_mult.c
	$(CC) $(CFLAGS) $(OPENMP_FLAGS) -o $@ $<

openmp_pi: openmp_pi.c
	$(CC) $(CFLAGS) $(OPENMP_FLAGS) -o $@ $<

# MPI targets
mpi: $(MPI_TARGETS)

mpi_vector_add: mpi_vector_add.c
	$(MPICC) $(CFLAGS) -o $@ $<

mpi_matrix_mult: mpi_matrix_mult.c
	$(MPICC) $(CFLAGS) -o $@ $<

mpi_pi: mpi_pi.c
	$(MPICC) $(CFLAGS) -o $@ $<

# Run targets
run-openmp: openmp
	@echo "Running OpenMP examples..."
	@echo "Vector Addition:"
	./openmp_vector_add
	@echo "\nMatrix Multiplication:"
	./openmp_matrix_mult
	@echo "\nPi Calculation:"
	./openmp_pi

run-mpi: mpi
	@echo "Running MPI examples with 4 processes..."
	@echo "Vector Addition:"
	mpirun -np 4 ./mpi_vector_add
	@echo "\nMatrix Multiplication:"
	mpirun -np 4 ./mpi_matrix_mult
	@echo "\nPi Calculation:"
	mpirun -np 4 ./mpi_pi

run-mpi-2: mpi
	@echo "Running MPI examples with 2 processes..."
	@echo "Vector Addition:"
	mpirun -np 2 ./mpi_vector_add
	@echo "\nMatrix Multiplication:"
	mpirun -np 2 ./mpi_matrix_mult
	@echo "\nPi Calculation:"
	mpirun -np 2 ./mpi_pi

# Performance comparison
compare: openmp mpi
	@echo "=== Performance Comparison ==="
	@echo "OpenMP Results:"
	@echo "Vector Addition:"
	./openmp_vector_add
	@echo "\nMatrix Multiplication:"
	./openmp_matrix_mult
	@echo "\nPi Calculation:"
	./openmp_pi
	@echo "\nMPI Results (4 processes):"
	@echo "Vector Addition:"
	mpirun -np 4 ./mpi_vector_add
	@echo "\nMatrix Multiplication:"
	mpirun -np 4 ./mpi_matrix_mult
	@echo "\nPi Calculation:"
	mpirun -np 4 ./mpi_pi

# Clean up
clean:
	rm -f $(OPENMP_TARGETS) $(MPI_TARGETS)

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build all examples"
	@echo "  openmp       - Build OpenMP examples"
	@echo "  mpi          - Build MPI examples"
	@echo "  run-openmp   - Run all OpenMP examples"
	@echo "  run-mpi      - Run all MPI examples with 4 processes"
	@echo "  run-mpi-2    - Run all MPI examples with 2 processes"
	@echo "  compare      - Run performance comparison"
	@echo "  clean        - Remove all executables"
	@echo "  help         - Show this help message"

# Phony targets
.PHONY: all openmp mpi run-openmp run-mpi run-mpi-2 compare clean help
