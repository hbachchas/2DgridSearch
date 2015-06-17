#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "headers.h"

using namespace searchEnv;

//@function     testing function
void testor(void);

//@function     prepares the environment from the config file, for the search to proceed
int prepareEnvironment(QString configFile, Environment& env);
//@function     inserts node as per priority
void insertInList(QList<POSITION*> &openlist, POSITION* current);
//@function     find forward heuristic value: straight line distance
MYINT hvalueForward(POSITION * current, Environment* env);
//@function     find backward heuristic value: straight line distance
MYINT hvalueBackward(POSITION * current, Environment *env);
//@function     find cost between two nodes
MYINT cost(POSITION * node1, POSITION *node2);
// returns true if f1>f2 upto 3 decimal places
bool compareFloatValues(double f1, double f2);
//@function     finds children of current node
void findChildren_(QVector<POSITION *> *pVect, POSITION* current, Environment* env);
//@function     ARA* fvalue forward
MYINT fvalueForward(POSITION* current, Environment *env, int *epsilon);
//@function     ARA* fvalue backward
MYINT fvalueBackward(POSITION* current, Environment *env, int *epsilon);
//@function     second function of ARA*
int improvePath(POSITION *goal, int epsilon, QList<POSITION *> *openlist, QHash<QString, POSITION*> *inconslist, Environment *env);
//@function     second function of BARA*
int improvePathBidir(int epsilon, QList<POSITION *> *openlistForward, QHash<QString, POSITION *> *inconslistForward,
                     QList<POSITION *> *openlistBackward, QHash<QString, POSITION*> *inconslistBackward, Environment *env);
//@function     printPath
void printARAstarPath(POSITION *goal, Environment* env, QTime *t);
void printBidirAstarPath(POSITION *current, Environment* env, QTime *t, bool common);
//@function     return a dynamically allocated 2D array
template <typename T>
T **AllocateDynamicArray(int nRows, int nCols);
//@function     delete the dynamic memory allocated to a 2D array
template <typename T>
void FreeDynamicArray(T** dArray);
//@function Write image file
void composeImageFile(QImage *image, bool lastRound, QString imageFilePath, int epsilon);
//@function     create QImage for newly generated states
void perPixelGridPainter(QImage *image, QPoint *ptMe, bool lastRound, bool direction, QString imageFilePath, int epsilon);
//@function     create QImage with intial config file
void oneTimeGridPainter(QImage *image, GRIDSTATE **displayGrid,
                        int IMAGE_X, int IMAGE_Y, int startX, int startY, int goalX, int goalY, QString imageFilePath);
int assertCondition(int condType);
bool checkPathCorrectness(Environment *env, POSITION *startState, POSITION *goalState, bool forwardSearch);
QImage composeZoomedINimageFile(QImage *image);
QImage composeZoomedOUTimageFile(QImage *image);











#endif // FUNCTIONS_H
