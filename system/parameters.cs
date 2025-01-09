
// **Define Parameters for Mesh Dimensions**
xMin -6;
xMax  12;

yMin -7;
yMax  7;

zMin -7;
zMax  7;

// **Define Number of Cells**
cellSize 0.1;  // Adjust cell size as needed


Ux 100;
Uy 0.0;
Uz 0.0;
P 69681;
T 268;

WriteInterval 40;


refinementRegionsLevels ((0.1 5) (0.2 3) (0.4 2) (0.8 1));