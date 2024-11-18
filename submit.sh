#!/bin/bash


module load openfoam/11


foamCleanTutorials

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