#!/bin/bash


rm -r 0

cp -r 0.orig 0
# Source the OpenFOAM environment (modify as needed)
if module avail openfoam/v2406 &>/dev/null; then
    module purge
    module load openfoam/v2406
    numProcs=$SLURM_NTASKS  # Server
    echo "OpenFOAM v12 module loaded successfully."
else
    op2406
    numProcs=4  # Local machine
    #op11
    echo "Using OpenFOAM op11."
fi

cd ${0%/*} || exit 1    # Run from this directory


# ============================
# Modify decomposeParDict
# ============================

echo "Updating decomposeParDict for $numProcs processors..."
foamDictionary system/decomposeParDict -entry "numberOfSubdomains" -set "$numProcs"


renumberMesh -overwrite 2>&1 | tee logs/renumberMesh.log;

#Speed steps
decomposePar 2>&1  | tee ./logs/decomposePar2.log;


echo "Checking the mesh quality..."

mpirun -np $numProcs checkMesh -parallel 2>&1 | tee logs/checkMesh.log;


mpirun -np $numProcs checkMesh -allGeometry -allTopology -parallel 2>&1 | tee logs/checkMeshAllTopology.log

# Step 5: Create foam.foam for visualization
echo "Creating foam.foam for visualization..."
for d in processor*; do touch "$d/foam.foam"; done


# Step 6: Run the solver in parallel
echo "Running rhoPimpleFoam in parallel..."
mpirun -np $numProcs rhoSimpleFoam -parallel 2>&1 | tee logs/rhoSimpleFoam.log


reconstructPar -latestTime 2>&1 | tee logs/log.reconstructPar