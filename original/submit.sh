#!/bin/bash
#SBATCH --job-name=CR25
#SBATCH --mem=68G
#SBATCH --nodes=1
#SBATCH --ntasks=64
#SBATCH --cpus-per-task=1
#SBATCH --time=20:0:0    
#SBATCH --mail-user=darasamii@gmail.com
#SBATCH --mail-type=ALL
#SBATCH --account=def-tembelym
#SBATCH --output=submit.log


./Mesh.sh
#./MeshPractice.sh
./Allrun.sh

