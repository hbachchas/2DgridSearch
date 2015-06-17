#ifndef HEAP_H
#define HEAP_H

#include "headers.h"

void writeOutput2File(QPoint *pt, MYINT fvalue);        // write the console output to a file
int validateHeap(QList<POSITION*> *openlist, POSITION *current = NULL, QList<POSITION*> *openlistcopy = NULL,
                 QList<POSITION*> *decreaseKeyNodeChain = NULL);          // validate the heap
void recursiveValidateHeap(QList<POSITION*> *openlist, int pos);    // recursive function to validate heap

void minHeapifyF(QList<POSITION*> *openlist, int i, Environment *env);
void buildMinHeapF(QList<POSITION*> *openlist, Environment *env);
//@function     insert current in openlist heap     2*O(lg n)
void minHeapInsertF(QList<POSITION*> *openlist, POSITION* current, Environment *env, QList<POSITION*> *decreaseKeyNodeChain = NULL);
//@function     extract minimum     O(lg n)
POSITION* extractMinF(QList<POSITION*> *openlist, Environment *env);
//@function     decrease key of an element at position pos      O(lg n)
void heapDecreaseKeyF(QList<POSITION*> *openlist, int pos, Environment *env, QList<POSITION*> *decreaseKeyNodeChain = NULL);
//@function     check if current exists in heap, return position        O(lg n)
int heapExistsKeyF(QList<POSITION*> *openlist, POSITION *current);


void minHeapifyB(QList<POSITION*> *openlist, int i, Environment *env);
void buildMinHeapB(QList<POSITION*> *openlist, Environment *env);
//@function     insert current in openlist heap     2*O(lg n)
void minHeapInsertB(QList<POSITION*> *openlist, POSITION* current, Environment *env);
//@function     extract minimum     O(lg n)
POSITION* extractMinB(QList<POSITION*> *openlist, Environment *env);
//@function     decrease key of an element at position pos      O(lg n)
void heapDecreaseKeyB(QList<POSITION*> *openlist, int pos, Environment *env);
//@function     check if current exists in heap, return position        O(lg n)
int heapExistsKeyB(QList<POSITION*> *openlist, POSITION *current);


#endif // HEAP_H

