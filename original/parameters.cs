
// **Define Parameters for Mesh Dimensions**
xMin -5;
xMax  14;

yMin -5;
yMax  5;

zMin -5;
zMax  5;

// **Define Number of Cells**
cellSize 0.2;  // Adjust cell size as needed
gradingFactor 10.0;

// Margins and lengths
marginLength 0.5;
lengthRocket 3.0;
yzMargin 0.5;


P 69681;
T 268;


Ma      0.8;
T       288.15;
gamma   1.4;
R       287.05;


AoA 10;

WriteInterval 40;


refinementRegionsLevels ((0.1 3) (0.2 2) (0.4 1));