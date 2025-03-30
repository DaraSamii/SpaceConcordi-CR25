#!/bin/bash


# Source the OpenFOAM environment (modify as needed)
if module avail openfoam/12 &>/dev/null; then
    module purge
    module load openfoam/12
    numProcs=$SLURM_NTASKS  # Server
    echo "OpenFOAM v12 module loaded successfully."
else
    op11
    numProcs=4  # Local machine

    #op11
    echo "Using OpenFOAM op11."
fi

cd ${0%/*} || exit 1    # Run from this directory

# Source tutorial run functions
. $WM_PROJECT_DIR/bin/tools/RunFunctions

# Source tutorial clean functions
. $WM_PROJECT_DIR/bin/tools/CleanFunctions

cleanCase

rm -r ./0 ./processor*/

cp -r ./0.orig ./0

# Remove and recreate logs directory
if [ -d "logs" ]; then
    echo "Removing existing logs directory..."
    rm -r logs
fi

mkdir -p logs
echo "Logs directory created."

# Touch foam.foam for ParaView compatibility
touch foam.foam

# ============================
# Modify decomposeParDict
# ============================

echo "Updating decomposeParDict for $numProcs processors..."
foamDictionary system/decomposeParDict -entry "numberOfSubdomains" -set "$numProcs"

#------------------------------------------------------------------------------
# Step 1: Generate base mesh
blockMesh 2>&1 | tee ./logs/blockMesh.log;

# Step 2: Extract surface features 
# IMPORTANT : it should be done with surfaceFeature of openfoam11 or openfoam12. v2406 doesn't work
#surfaceFeatureExtract 2>&1  | tee ./logs/log.surfaceFeatures;
#gzip -d constant/triSurface/file.gz
# Step 3: Decompose the domain for parallel processing
decomposePar -copyZero 2>&1  | tee ./logs/decomposePar.log;

# Step 4: Run snappyHexMesh in parallel on 4 cores
mpirun -np $numProcs snappyHexMesh -overwrite -parallel 2>&1  | tee ./logs/snappyHexMesh.log;

#mpirun -np $numProcs snappyHexMesh -dict ./system/snappyHexMeshDictLayer -overwrite -parallel 2>&1  | tee ./logs/log.snappyHexMeshLayer;

reconstructParMesh -constant 2>&1  | tee ./logs/reconstructParMesh.log;

rm -r processor*