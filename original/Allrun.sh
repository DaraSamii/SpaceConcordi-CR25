#!/bin/bash

# ============================
# OpenFOAM v2406 Automation Script
# blockMesh → snappy → reconstruct → topoSet → run
# ============================


cd "$(dirname "$0")" || exit 1
echo "Running in: $(pwd)"


#----------------------------------------
# Load OpenFOAM v2406 Environment
#----------------------------------------
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

#----------------------------------------
# Clean Case
#----------------------------------------
echo "Cleaning case..."
rm -rf 0 processor*
cp -r 0.orig 0

mkdir -p logs
touch foam.foam

#----------------------------------------
# Step 1: blockMesh
#echo "Running blockMesh..."
#blockMesh > logs/blockMesh.log 2>&1


# IMPORTANT : it should be done with surfaceFeature of openfoam11 or openfoam12. v2406 doesn't work
#surfaceFeatureExtract 2>&1  | tee ./logs/log.surfaceFeatures;
#gzip -d constant/triSurface/file.gz


#----------------------------------------
# Step 2: Initial decompose (for snappyHexMesh)
#echo "Decomposing for snappyHexMesh..."
#foamDictionary system/decomposeParDict -entry "numberOfSubdomains" -set "$numProcs"
#decomposePar -copyZero > logs/decomposePar_preSnappy.log 2>&1

#----------------------------------------
# Step 3: snappyHexMesh
#echo "Running snappyHexMesh in parallel..."
#mpirun -np $numProcs snappyHexMesh -overwrite -parallel > logs/snappyHexMesh.log 2>&1

#----------------------------------------
# Step 4: Reconstruct mesh
#echo "Reconstructing mesh after snappyHexMesh..."
#reconstructParMesh -constant > logs/reconstructParMesh.log 2>&1
#rm -rf processor*

#----------------------------------------
# Step 5: topoSet (after reconstructed mesh)
echo "Running topoSet on reconstructed mesh..."
topoSet > logs/topoSet.log 2>&1

#----------------------------------------
# Step 6: Renumber
echo "Renumbering mesh..."
renumberMesh -overwrite > logs/renumberMesh.log 2>&1

#----------------------------------------
# Step 7: Decompose final mesh
echo "Decomposing final mesh..."
foamDictionary system/decomposeParDict -entry "numberOfSubdomains" -set "$numProcs"
decomposePar > logs/decomposePar.log 2>&1

#----------------------------------------
# Step 8: checkMesh
echo "Running checkMesh..."
mpirun -np $numProcs checkMesh -parallel > logs/checkMesh.log 2>&1
mpirun -np $numProcs checkMesh -allGeometry -allTopology -parallel > logs/checkMeshAll.log 2>&1

#----------------------------------------
# Step 9: foam.foam for ParaView
echo "Creating foam.foam..."
for d in processor*; do touch "$d/foam.foam"; done


#----------------------------------------
cp ./system/fvSchemes.stable ./system/fvSchemes 

#----------------------------------------
# Step 10: Solver
echo "Running rhoSimpleFoam in parallel..."
mpirun -np $numProcs rhoSimpleFoam -parallel > logs/rhoSimpleFoam.log 2>&1

#----------------------------------------
# Step 11: Reconstruct results
echo "Reconstructing latest time..."
reconstructPar -latestTime > logs/reconstructPar.log 2>&1

#----------------------------------------
# Step 12: Post-process
echo "Sampling rocket patch to VTK..."
postProcess -func rocketVTK -latestTime > logs/rocketVTK.log 2>&1

