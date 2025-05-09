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

echo "Creating logs directory..."
mkdir -p logs

echo "Creating foam.foam file for ParaView compatibility..."
touch foam.foam

echo "Running blockMesh and logging to logs/blockMesh.log..."
blockMesh > logs/blockMesh.log 2>&1

echo "Extracting boundary patches to STL (patches.stl)..."
foamToSurface -tri -constant ./geometry/patches.stl

echo "Renaming solid and endsolid entries in rocket.stl to 'rocket'..."
sed -i 's/^solid .*/solid rocket/; s/^endsolid .*/endsolid rocket/' ./geometry/rocket.stl

echo "Combining rocket.stl and patches.stl into combined.stl..."
cat ./geometry/rocket.stl ./geometry/patches.stl > ./geometry/combined.stl

echo "Converting combined.stl to FMS format (combined.fms)..."
surfaceToFMS ./geometry/combined.stl

echo "Running cartesianMesh and logging to logs/cartesianMesh.log..."
cartesianMesh > logs/cartesianMesh.log 2>&1

echo "Mesh generation pipeline completed successfully."