#!/bin/bash
#SBATCH --job-name=CR25
#SBATCH --mem=68G
#SBATCH --nodes=1
#SBATCH --ntasks=64
#SBATCH --cpus-per-task=1
#SBATCH --time=10:0:0    
#SBATCH --mail-user=darasamii@gmail.com
#SBATCH --mail-type=ALL
#SBATCH --account=def-tembelym
#SBATCH --output=submit.log


#./MeshPractice.sh
#./Allrun.sh

#cp ./system/fvSchemes.stable ./system/fvSchemes 

echo "Loading OpenFOAM v2406 environment..."
if module avail openfoam/v2406 &>/dev/null; then
    module purge
    source ~/v2406/OpenFOAM-v2406/etc/bashrc
    source ~/OpenFOAM/OpenFOAM-v2406/etc/bashrc
    numProcs=$SLURM_NTASKS
    echo "OpenFOAM v2406 loaded via module with $numProcs processors."
else
    source /opt/openfoam2406/etc/bashrc
    numProcs=4
    echo "OpenFOAM v2406 loaded from /opt with $numProcs processors."
fi


mpirun -np $numProcs rhoSimpleFoam -parallel > logs/rhoSimpleFoam.log 2>&1
