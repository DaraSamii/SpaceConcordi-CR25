
// **Define Parameters for Mesh Dimensions**
xMin -6;
xMax  8;

yMin -5;
yMax  5;

zMin -5;
zMax  5;

// **Define Number of Cells**
cellSize 0.2;  // Adjust cell size as needed


Ux 600;
Uy 0.0;
Uz 0.0;
P 100000;
T 300;

maxCo 0.5;
deltaT 2e-07;
maxDeltaT 5e-06;


firstLayerThickness 0.1;
minThickness    1e-5;

refinementRegionsLevels ((0.3 3) (0.6 2) (1.2 1));// (2.4 1));