# SLURM Quick Reference Card

## Essential Commands

### Check Status
```bash
# Cluster status
sinfo

# Your jobs
squeue -u $USER

# All jobs
squeue

# Job details
scontrol show job <job_id>
```

### Submit Jobs
```bash
# Submit script
sbatch job_script.sh

# Submit with options
sbatch -p gpu -t 2:00:00 --mem=8G job_script.sh

# Run command directly
srun -p compute -t 10:00 --mem=4G python script.py

# Interactive session
srun -p interactive -t 1:00:00 --mem=8G --pty bash
```

### Manage Jobs
```bash
# Cancel job
scancel <job_id>

# Cancel all your jobs
scancel -u $USER

# Cancel by name
scancel --name="my_job"
```

## Common Job Script Headers

### Basic Job
```bash
#!/bin/bash
#SBATCH --job-name=my_job
#SBATCH --output=output_%j.txt
#SBATCH --error=error_%j.txt
#SBATCH --time=01:00:00
#SBATCH --partition=compute
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=8G
```

### GPU Job
```bash
#!/bin/bash
#SBATCH --job-name=gpu_job
#SBATCH --output=gpu_%j.txt
#SBATCH --error=gpu_%j.txt
#SBATCH --time=02:00:00
#SBATCH --partition=gpu
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=16G
#SBATCH --gres=gpu:1
```

### Array Job
```bash
#!/bin/bash
#SBATCH --job-name=array_job
#SBATCH --output=array_%A_%a.txt
#SBATCH --error=array_%A_%a.txt
#SBATCH --time=01:00:00
#SBATCH --partition=compute
#SBATCH --array=1-10
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=4G
```

## Resource Specifications

### Time Formats
- `--time=01:30:00` (1 hour 30 minutes)
- `--time=30:00` (30 minutes)
- `--time=1-12:30:00` (1 day 12 hours 30 minutes)

### Memory
- `--mem=8G` (8 GB total)
- `--mem-per-cpu=2G` (2 GB per CPU)
- `--mem-per-node=32G` (32 GB per node)

### CPUs and Tasks
- `--cpus-per-task=4` (4 CPUs per task)
- `--ntasks=8` (8 total tasks)
- `--nodes=2` (2 nodes)

### GPU
- `--gres=gpu:1` (1 GPU)
- `--gres=gpu:2` (2 GPUs)
- `--gres=gpu:v100:1` (1 V100 GPU)

## Environment Variables

- `$SLURM_JOB_ID` - Job ID
- `$SLURM_JOB_NAME` - Job name
- `$SLURM_ARRAY_TASK_ID` - Array task ID
- `$SLURM_SUBMIT_DIR` - Submit directory
- `$SLURM_NTASKS` - Number of tasks
- `$SLURM_CPUS_PER_TASK` - CPUs per task

## Monitoring

```bash
# Job accounting
sacct -j <job_id>

# Detailed accounting
sacct -j <job_id> --format=JobID,JobName,State,ExitCode,Start,End,Elapsed,MaxRSS

# Job history
sacct -u $USER --starttime=2024-01-01
```

## Common Patterns

### Submit and Monitor
```bash
# Submit job
JOB_ID=$(sbatch job_script.sh | awk '{print $4}')

# Monitor
watch -n 5 "squeue -u $USER"

# Check output
tail -f output_${JOB_ID}.txt
```

### Array Job Pattern
```bash
#!/bin/bash
#SBATCH --array=1-100
#SBATCH --output=job_%A_%a.out

TASK_ID=$SLURM_ARRAY_TASK_ID
python process.py --input data_${TASK_ID}.txt --output result_${TASK_ID}.txt
```

### Dependency Jobs
```bash
# Submit first job
JOB1=$(sbatch job1.sh | awk '{print $4}')

# Submit dependent job
sbatch --dependency=afterok:$JOB1 job2.sh
```
