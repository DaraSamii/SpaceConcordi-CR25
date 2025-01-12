#!/bin/bash
#SBATCH --job-name=CR25
#SBATCH --mem=48G
#SBATCH --nodes=1
#SBATCH --ntasks=36
#SBATCH --cpus-per-task=1
#SBATCH --time=8:0:0    
#SBATCH --mail-user=darasamii@gmail.com
#SBATCH --mail-type=ALL
#SBATCH --account=def-tembelym
#SBATCH --output=submit.log



./Allrun.sh