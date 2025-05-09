#!/bin/bash


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



blockMesh > logs/blockMesh.log 2>&1


foamToSurface -tri -constant ./geometry/patches.stl   


sed -i 's/^solid .*/solid rocket/; s/^endsolid .*/endsolid rocket/' ./geometry/rocket.stl

cat ./geometry/rocket.stl ./geometry/patches.stl > ./geometry/combined.stl

surfaceToFMS ./geometry/combined.stl   

cartesianMesh