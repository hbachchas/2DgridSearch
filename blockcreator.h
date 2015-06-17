#ifndef BLOCKCREATOR_H
#define BLOCKCREATOR_H

//@function True if the block lies within the legible permiter
bool isPlaceable(int **grid, int startNodeX, int startNodeY, int theta);
// LShpae blocks test
bool isPlaceableLShape(int startNodeX, int startNodeY, int type);
bool placeBlockLShape(int **grid, int startNodeX, int startNodeY, int type, int xDist, int yDist);
//@function Place the block in the grid
bool placeBlock(int **grid, int startNodeX, int startNodeY, int theta);
//@function     create QImage
void gridPainter(int **displayGrid, int startX, int startY, int goalX, int goalY, QString path, int fileCount, bool silent);
//@function Draw inverted C configuration
void getInvertedCBlock(int **grid, int d);
template <typename T>
void FreeDynamicArray_blockcreator(T** dArray);

bool isPlaceableOutdoorShape(int **grid, int startNodeX, int startNodeY, int type, int xDist, int yDist);
bool placeBlockOutdoorShape(int **grid, int startNodeX, int startNodeY, int type, int xDist, int yDist);


#endif // BLOCKCREATOR_H

