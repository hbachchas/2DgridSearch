#ifndef FILEFORMATTERUTILITY_H
#define FILEFORMATTERUTILITY_H

//@function     Random number generator
int getRandomNumber(int low, int high);
//@function     Prepare environment from IN-Memory grid variable
void prepareAstarEnvironment(Environment &env, int **grid);
//@function     Write random generated search space in a FILE
int composeSearchSpaceFile(int **grid, QString path, int fileNumber, int startNodeX, int startNodeY, int goalNodeX, int goalNodeY);
//@function     Read two files, Merge data horizontally (side to side)
void mergeFiles();

#endif // FILEFORMATTERUTILITY_H

