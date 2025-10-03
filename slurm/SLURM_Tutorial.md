# SLURM Tutorial: Complete Guide with Shell Examples

## Table of Contents

1. [What is SLURM?](#what-is-slurm)
2. [Basic SLURM Commands](#basic-slurm-commands)
3. [Job Submission](#job-submission)
4. [Job Scripts](#job-scripts)
5. [Resource Management](#resource-management)
6. [Job Monitoring](#job-monitoring)
7. [Array Jobs](#array-jobs)
8. [Interactive Jobs](#interactive-jobs)
9. [Common Use Cases](#common-use-cases)
10. [Troubleshooting](#troubleshooting)

## What is SLURM?

**SLURM (Simple Linux Utility for Resource Management)** is a job scheduler and resource manager for Linux clusters. It manages computational resources and schedules jobs to run on available nodes.

### Key Concepts:

- **Job**: A computational task submitted to the cluster
- **Node**: A physical computer in the cluster
- **Partition**: A group of nodes with specific characteristics
- **Queue**: A collection of jobs waiting to be executed

## Basic SLURM Commands

### Check Cluster Status

```bash
# View cluster information
sinfo

# View detailed partition information
sinfo -l

# Check specific partition
sinfo -p gpu

# View node details
scontrol show nodes
```

### Check Your Jobs

```bash
# List your jobs
squeue -u $USER

# List all jobs
squeue

# List jobs in specific partition
squeue -p gpu

# Show detailed job information
scontrol show job <job_id>
```

### Cancel Jobs

```bash
# Cancel a specific job
scancel <job_id>

# Cancel all your jobs
scancel -u $USER

# Cancel jobs by name
scancel --name="my_job"
```

## Job Submission

### Basic Job Submission

```bash
# Submit a simple job
sbatch job_script.sh

# Submit with specific partition
sbatch -p gpu job_script.sh

# Submit with time limit
sbatch -t 01:30:00 job_script.sh

# Submit with memory requirement
sbatch --mem=8G job_script.sh

# Submit with multiple cores
sbatch -c 4 job_script.sh
```

### Command Line Job Submission

```bash
# Run a single command
srun -p gpu -t 10:00 --mem=4G python script.py

# Run with specific number of tasks
srun -n 4 -p compute --mem=8G mpirun -np 4 ./my_program

# Run interactively
srun -p interactive -t 2:00:00 --mem=4G --pty bash
```

## Job Scripts

### Basic Job Script Template

```bash
#!/bin/bash
#SBATCH --job-name=my_job
#SBATCH --output=output_%j.txt
#SBATCH --error=error_%j.txt
#SBATCH --time=01:00:00
#SBATCH --partition=compute
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=8G

# Load modules (if using module system)
module load python/3.9
module load gcc/9.3.0

# Change to working directory
cd $SLURM_SUBMIT_DIR

# Run your program
python my_script.py
```

### GPU Job Script

```bash
#!/bin/bash
#SBATCH --job-name=gpu_job
#SBATCH --output=gpu_output_%j.txt
#SBATCH --error=gpu_error_%j.txt
#SBATCH --time=02:00:00
#SBATCH --partition=gpu
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=16G
#SBATCH --gres=gpu:1

# Load CUDA module
module load cuda/11.0

# Run GPU program
python train_model.py
```

### MPI Job Script

```bash
#!/bin/bash
#SBATCH --job-name=mpi_job
#SBATCH --output=mpi_output_%j.txt
#SBATCH --error=mpi_error_%j.txt
#SBATCH --time=01:30:00
#SBATCH --partition=compute
#SBATCH --nodes=2
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=2
#SBATCH --mem=4G

# Load MPI module
module load openmpi/4.0.0

# Run MPI program
mpirun -np 8 ./my_mpi_program
```

## Resource Management

### Memory Management

```bash
# Request specific memory amount
#SBATCH --mem=8G

# Request memory per CPU
#SBATCH --mem-per-cpu=2G

# Request memory per node
#SBATCH --mem-per-node=32G
```

### CPU Management

```bash
# Request specific number of CPUs
#SBATCH --cpus-per-task=4

# Request total number of tasks
#SBATCH --ntasks=8

# Request number of nodes
#SBATCH --nodes=2
```

### Time Management

```bash
# Time format: HH:MM:SS
#SBATCH --time=01:30:00

# Time format: MM:SS
#SBATCH --time=30:00

# Time format: days-hours:minutes:seconds
#SBATCH --time=1-12:30:00
```

## Job Monitoring

### Check Job Status

```bash
# Basic job status
squeue -u $USER

# Detailed job information
scontrol show job <job_id>

# Job accounting information
sacct -j <job_id>

# Job accounting with details
sacct -j <job_id> --format=JobID,JobName,State,ExitCode,Start,End,Elapsed,MaxRSS,MaxVMSize
```

### Monitor Job Output

```bash
# Follow job output in real-time
tail -f output_<job_id>.txt

# Check job logs
cat output_<job_id>.txt
cat error_<job_id>.txt
```

## Array Jobs

### Basic Array Job

```bash
#!/bin/bash
#SBATCH --job-name=array_job
#SBATCH --output=array_%A_%a.out
#SBATCH --error=array_%A_%a.err
#SBATCH --time=01:00:00
#SBATCH --partition=compute
#SBATCH --array=1-10
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=4G

# Get array task ID
TASK_ID=$SLURM_ARRAY_TASK_ID

# Process different files based on task ID
python process_data.py --input data_${TASK_ID}.txt --output result_${TASK_ID}.txt
```

### Array Job with Step Size

```bash
#!/bin/bash
#SBATCH --job-name=array_step
#SBATCH --output=array_%A_%a.out
#SBATCH --error=array_%A_%a.err
#SBATCH --time=01:00:00
#SBATCH --partition=compute
#SBATCH --array=1-20:2
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=4G

# This creates tasks: 1, 3, 5, 7, 9, 11, 13, 15, 17, 19
TASK_ID=$SLURM_ARRAY_TASK_ID
echo "Processing task $TASK_ID"
```

## Interactive Jobs

### Interactive Session

```bash
# Request interactive session
srun -p interactive -t 2:00:00 --mem=8G --cpus-per-task=4 --pty bash

# Interactive session with GPU
srun -p gpu -t 1:00:00 --mem=16G --gres=gpu:1 --pty bash

# Interactive session on specific node
srun -p compute -w node001 -t 1:00:00 --mem=8G --pty bash
```

## Common Use Cases

### 1. Machine Learning Training

```bash
#!/bin/bash
#SBATCH --job-name=ml_training
#SBATCH --output=training_%j.out
#SBATCH --error=training_%j.err
#SBATCH --time=4:00:00
#SBATCH --partition=gpu
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=32G
#SBATCH --gres=gpu:2

module load python/3.9
module load cuda/11.0

# Activate virtual environment
source venv/bin/activate

# Run training
python train_model.py --epochs 100 --batch-size 64 --gpus 2
```

### 2. Scientific Computing

```bash
#!/bin/bash
#SBATCH --job-name=scientific_compute
#SBATCH --output=sci_%j.out
#SBATCH --error=sci_%j.err
#SBATCH --time=6:00:00
#SBATCH --partition=compute
#SBATCH --nodes=4
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=2
#SBATCH --mem=8G

module load openmpi/4.0.0
module load gcc/9.3.0

# Run parallel scientific computation
mpirun -np 16 ./scientific_program input.dat output.dat
```

### 3. Data Processing Pipeline

```bash
#!/bin/bash
#SBATCH --job-name=data_pipeline
#SBATCH --output=pipeline_%j.out
#SBATCH --error=pipeline_%j.err
#SBATCH --time=2:00:00
#SBATCH --partition=compute
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=16G

module load python/3.9

# Run data processing pipeline
python preprocess_data.py
python analyze_data.py
python generate_report.py
```

## Troubleshooting

### Common Issues and Solutions

#### 1. Job Pending Forever

```bash
# Check why job is pending
scontrol show job <job_id>

# Check partition availability
sinfo -p <partition_name>

# Check resource availability
sinfo -N -l
```

#### 2. Job Fails Immediately

```bash
# Check error logs
cat error_<job_id>.txt

# Check if modules are loaded correctly
module list

# Check if files exist
ls -la input_files/
```

#### 3. Out of Memory Errors

```bash
# Increase memory request
#SBATCH --mem=16G

# Or use memory per CPU
#SBATCH --mem-per-cpu=4G
```

#### 4. Time Limit Exceeded

```bash
# Increase time limit
#SBATCH --time=4:00:00

# Or check job efficiency
sacct -j <job_id> --format=JobID,JobName,State,Elapsed,MaxRSS
```

### Useful Commands for Debugging

```bash
# Check job history
sacct -u $USER --starttime=2024-01-01

# Check partition limits
scontrol show partition <partition_name>

# Check node status
scontrol show node <node_name>

# Check job dependencies
scontrol show job <job_id> | grep -i depend
```

## Best Practices

### 1. Resource Estimation

- Always request slightly more resources than needed
- Use `sacct` to analyze past job resource usage
- Start with conservative estimates and adjust

### 2. Job Organization

- Use descriptive job names
- Organize output files in directories
- Use job arrays for similar tasks

### 3. Error Handling

- Always check error logs
- Use proper exit codes in scripts
- Implement checkpointing for long jobs

### 4. Efficiency Tips

- Use appropriate partition for your job type
- Avoid requesting more resources than needed
- Use job arrays instead of many individual jobs
- Clean up temporary files

## Example Workflow

```bash
# 1. Create job script
cat > my_job.sh << 'EOF'
#!/bin/bash
#SBATCH --job-name=example
#SBATCH --output=example_%j.out
#SBATCH --error=example_%j.err
#SBATCH --time=01:00:00
#SBATCH --partition=compute
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=8G

echo "Job started at: $(date)"
echo "Running on node: $(hostname)"
echo "Job ID: $SLURM_JOB_ID"

# Your actual work here
python my_script.py

echo "Job completed at: $(date)"
EOF

# 2. Submit job
sbatch my_job.sh

# 3. Monitor job
squeue -u $USER

# 4. Check results
cat example_<job_id>.out
```

This tutorial covers the essential SLURM commands and concepts you'll need for cluster computing. Remember to check your specific cluster's documentation for any custom configurations or available modules.
