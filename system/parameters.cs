
// **Define Parameters for Mesh Dimensions**
xMin -7;
xMax  10;

yMin -6;
yMax  6;

zMin -6;
zMax  6;

// **Define Number of Cells**
cellSize 0.1;  // Adjust cell size as needed


Ux 600;
Uy 148.16;

P 100000;
T 300;

maxCo 0.5;
deltaT 1e-06;
maxDeltaT 1e-05;


finalLayerThickness 1e-3;
minThickness    1e-5;

refinementRegionsLevels ((0.3 4) (0.6 3) (1.2 2));