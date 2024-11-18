#!/bin/bash
#SBATCH --job-name=CR25
#SBATCH --mem=32G
#SBATCH --nodes=1
#SBATCH --ntasks=9
#SBATCH --cpus-per-task=1
#SBATCH --time=12:0:0    
#SBATCH --mail-user=darasamii@gmail.com
#SBATCH --mail-type=ALL
#SBATCH --account=def-tembelym
#SBATCH --output=submit.out
module purge

module load openfoam/11


# Source tutorial clean functions
. $WM_PROJECT_DIR/bin/tools/CleanFunctions

cleanCase

mkdir -p logs
#------------------------------------------------------------------------------
# Step 1: Generate base mesh
blockMesh | tee log.blockMesh;

# Step 2: Extract surface features
surfaceFeatures | tee ./logs/log.surfaceFeatures;

# Step 3: Decompose the domain for parallel processing
decomposePar -copyZero | tee ./logs/log.decomposePar;

# Step 4: Run snappyHexMesh in parallel on 4 cores
srun snappyHexMesh -overwrite -parallel | tee ./logs/log.snappyHexMesh;

# Optional Step 5: Reconstruct the final mesh (combine into single mesh)
reconstructPar -constant | tee ./logs/log.reconstructParMesh;

srun foamRun -parallel | tee ./logs/log.foamRun; 

reconstructPar -constant | tee ./logs/log.reconstructPar;
