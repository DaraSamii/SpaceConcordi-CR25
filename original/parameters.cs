
// **Define Parameters for Mesh Dimensions**
xMin -5;
xMax 20;

yMin -10;
yMax  10;

zMin -10;
zMax  10;

// **Define Number of Cells**
maxCellSize 0.5;
rocketCellSize 0.001;  
firstLayerThickness 0.0001;

P 69681;
T 268;


Ma      0.8;
T       288.15;
gamma   1.4;
R       287.05;


AoA 10;

WriteInterval 10;


refinementRegionsLevels ((0.1 3) (0.2 2) (0.4 1));