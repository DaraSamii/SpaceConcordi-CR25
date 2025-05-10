
// **Define Parameters for Mesh Dimensions**
xMin -5;
xMax  14;

yMin -5;
yMax  5;

zMin -5;
zMax  5;

// **Define Number of Cells**
maxCellSize 0.2;
rocketCellSize 0.002;  
boxWidth 1;
boxLength 5;
boxCellSize 0.05;
firstLayerThickness 0.001;

P 69681;
T 268;


Ma      0.8;
T       288.15;
gamma   1.4;
R       287.05;


AoA 10;

WriteInterval 10;


refinementRegionsLevels ((0.1 3) (0.2 2) (0.4 1));