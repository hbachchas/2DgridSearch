#ifndef SEARCHCOMPARISION
#define SEARCHCOMPARISION

#include "headers.h"

//@function     second function of ARA*
int improvePathCCL(POSITION *goal, int epsilon, QList<POSITION *> *openlist, QHash<QString, POSITION*> *inconslist,
                   Environment *env, PRINTINFORMATION *printInfo);
//@function     second function of BARA*
int improvePathBidirCCL(int epsilon, QList<POSITION *> *openlistForward, QHash<QString, POSITION*> *inconslistForward, QList<POSITION *> *openlistBackward,
                        QHash<QString, POSITION*> *inconslistBackward, Environment *env, PRINTINFORMATION *printInfo);
int getMean(QList<int> tempValuesList);
int getStdDev(MYINT mean, QList<int> tempValuesList);
void findMeanStdDev();

#endif // SEARCHCOMPARISION

