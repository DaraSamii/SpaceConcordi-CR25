
// **Define Parameters for Mesh Dimensions**
xMin -5;
xMax  14;

yMin -5;
yMax  5;

zMin -5;
zMax  5;

// **Define Number of Cells**
cellSize 0.1;  // Adjust cell size as needed
gradingFactor 10.0;

// Margins and lengths
marginLength 0.5;
lengthRocket 3.0;
yzMargin 0.5;

Ux        408;
Uy        12.5;
Uz        0.0;
P        96115.5;
T        287.7;

WriteInterval 40;


refinementRegionsLevels ((0.1 3) (0.2 2) (0.4 1));