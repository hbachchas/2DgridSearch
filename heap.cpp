

#include "headers.h"

#define LEFT(i)     (2*i+1)
#define RIGHT(i)    (2*i+2)
#define PARENT(i)   (qFloor((i-1)/2))

using namespace searchEnv;

//@function     testing function
void testor(void)
{

    qDebug() << LEFT(2);
    qDebug() << RIGHT(2);
    qDebug() << PARENT(5);
    qDebug() << PARENT(6);

    QList<QPoint> openlist;
    openlist.push_back(QPoint(1,0));
    openlist.push_back(QPoint(0,1));
    openlist.push_back(QPoint(2,1));
    openlist.push_back(QPoint(2,3));
    openlist.push_back(QPoint(1,1));

    for (int i =0; i< openlist.size(); i++)
        qDebug() << openlist.at(i);

    for (int i=0; i< openlist.size(); i++)
    {
        for(int j=i+1; j< openlist.size(); j++)
        {
            if (openlist.at(i).x()>openlist.at(j).x())
                openlist.swap(i,j);
            else if ((openlist.at(i).x()==openlist.at(j).x()) && openlist.at(i).y()>openlist.at(j).y())
                openlist.swap(i,j);

        }
    }
    qDebug() << "-----------------";
    for (int i =0; i< openlist.size(); i++)
        qDebug() << openlist.at(i);

}

// for debugging heap
void writeOutput2File(QPoint *pt, MYINT fvalue)
{
    QString configFilePath = "/home/peace/qtConsole.out";
    QFile file(configFilePath);
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Error file opening the config file, Exiting... from composeSearchSpaceFile";
        return;
    }

    QTextStream out(&file);
    QString line = "";

    // line1
    line = "FH: (" + QString::number(pt->x()) + ", " + QString::number(pt->y()) + ")" + "  f = " + QString::number(fvalue);
    out << line << endl;

    out.flush();
    file.close();
}

// validate the heap
int validateHeap(QList<POSITION*> *openlist, POSITION *current, QList<POSITION *> *openlistcopy, QList<POSITION *> *decreaseKeyNodeChain)
{
#define PRINT_INCONSISTENCIES
    int inconsistencyCount = 0;

    // 2n recursive version
    //for( int i = 0;)

    // nlog(n) version
    int i=openlist->size()-1;
    int j=0;
    for (; i>0; i--)
    {
        // check i is ok
        for (j=i; j>0; j=PARENT(j))
        {
            if ( openlist->at(PARENT(j))->fForward > openlist->at(j)->fForward )
            {
#ifdef PRINT_INCONSISTENCIES
                // debug heap [13-June-15]
                QList<POSITION *> child2RootList_beforeNodeInsertion;
                if ( openlistcopy )
                {
                    for (int a = 0; a<openlistcopy->size(); a++)
                    {
                        if ( openlistcopy->at(a) == openlist->at(j) )
                        {
                            for (int b=a; b>=0; b=PARENT(b))
                            {
                                if ( b < openlistcopy->size() && b > 0 )
                                    child2RootList_beforeNodeInsertion.prepend( openlistcopy->at(b) );
                                else
                                    qDebug() << "Stuck up in heapValidate";
                            }
                        }
                    }
                }
                // debug heap [13-June-15]
                qDebug() << "FAILURE: Heap is inconsistent";
                qDebug() << "j = " << j << ",  node f = " << openlist->at(j)->fForward << ",    point = " << openlist->at(j)->ptMe;
                qDebug() << "PARENT(j) = " << PARENT(j) << ",  PARENT(node f) = " << openlist->at(PARENT(j))->fForward << ",    point = " << openlist->at(PARENT(j))->ptMe;
#endif
                inconsistencyCount++;
                break;
            }
        }
    }
    if (inconsistencyCount)
    {
#ifdef PRINT_INCONSISTENCIES
        qDebug() << "Total inconsistencies " << inconsistencyCount;
#endif
        return 1;   // Failure
    }
    return 0;   // Success
}

void recursiveValidateHeap(QList<POSITION*> *openlist, int pos)
{

}

//@function     insert current in openlist heap     2*O(lg n)
void minHeapInsertF(QList<POSITION*> *openlist, POSITION* current, Environment *env, QList<POSITION *> *decreaseKeyNodeChain)
{
    //register int pos = heapExistsKey(openlist, current);    // O(lg n)
    int pos = env->grid[current->ptMe.x()][current->ptMe.y()].heapIndexForward;
    if (pos > -1)       // if node already exist
        heapDecreaseKeyF(openlist, pos, env, decreaseKeyNodeChain);     // O(lg n)
    else
    {
        openlist->append(current);      // O(lg n)
        env->grid[current->ptMe.x()][current->ptMe.y()].heapIndexForward = openlist->size()-1;
        heapDecreaseKeyF(openlist, openlist->size()-1, env, decreaseKeyNodeChain);      // O(lg n)
    }
}

//@function     decrease key of an element at position pos      O(lg n)
void heapDecreaseKeyF(QList<POSITION*> *openlist, int pos, Environment *env, QList<POSITION *> *decreaseKeyNodeChain)
{
    MYINT key = openlist->at(pos)->fForward;
    //bool flag = true;   // debug heap [13-June-15]
    for(; pos > 0 && (openlist->at(PARENT(pos))->fForward > key); pos = PARENT(pos))
    {
#ifdef MYASSERT_VALIDATE_HEAP
        if ( PARENT(pos)>0 && decreaseKeyNodeChain )
        {
            if ( flag )
                decreaseKeyNodeChain->prepend( openlist->at(pos) ), flag = false;   // push child node // debug heap [13-June-15]
            decreaseKeyNodeChain->prepend( openlist->at(PARENT(pos)) );   // debug heap [13-June-15]
        }
#endif
        openlist->swap(pos, PARENT(pos));
        env->grid[openlist->at(PARENT(pos))->ptMe.x()][openlist->at(PARENT(pos))->ptMe.y()].heapIndexForward = PARENT(pos);
        env->grid[openlist->at(pos)->ptMe.x()][openlist->at(pos)->ptMe.y()].heapIndexForward = pos;
    }
    // tie breaking in favor of lower h_value
    /*if ( pos > 0 && openlist->at(PARENT(pos))->fForward == key && (openlist->at(PARENT(pos))->fForward-openlist->at(PARENT(pos))->gForward)
         > (openlist->at(pos)->fForward-openlist->at(pos)->gForward) )
    {
        openlist->swap(pos, PARENT(pos));
        env->grid[openlist->at(PARENT(pos))->ptMe.x()][openlist->at(PARENT(pos))->ptMe.y()].heapIndexForward = PARENT(pos);
        env->grid[openlist->at(pos)->ptMe.x()][openlist->at(pos)->ptMe.y()].heapIndexForward = pos;
    }*/
}

//@function     build min heap      O(n)
void buildMinHeapF(QList<POSITION*> *openlist, Environment *env)
{
    if (openlist->size())
    {
        // let everyone know its index
        for (int i=0; i<openlist->size(); i++)
            env->grid[openlist->at(i)->ptMe.x()][openlist->at(i)->ptMe.y()].heapIndexForward = i;

        for(int i = PARENT(openlist->size()-1); i>=0; --i)
            minHeapifyF(openlist, i, env);
    }
}

//@function     extract minimum     O(lg n)
POSITION* extractMinF(QList<POSITION*> *openlist, Environment *env)
{
    if (!openlist->size())
        return NULL;
    else
    {
        // not adjusting heapIndex of the node extracted, because it will not be used again in case of WA*
        openlist->swap(0, openlist->size()-1);      // swap first with last
        env->grid[openlist->at(0)->ptMe.x()][openlist->at(0)->ptMe.y()].heapIndexForward = 0;
        POSITION *temp = openlist->takeLast();

        // minHeapify at position 0
        minHeapifyF(openlist, 0, env);
        env->grid[temp->ptMe.x()][temp->ptMe.y()].heapIndexForward = -1;   // resetting needed in case of ARA*
#ifdef DEBUG_HEAP
        if (env->epsilon == 1)      // heap debug [12-June-15]
            writeOutput2File(&(temp->ptMe), temp->fForward);
#endif
        return temp;
    }
}

//@function     min Heapify openlist    O(lg n)
void minHeapifyF(QList<POSITION*> *openlist, int i, Environment *env)
{
    int smallest = -1;
    while(1)
    {
        if ( LEFT(i) < openlist->size() && (openlist->at(i)->fForward > openlist->at(LEFT(i))->fForward) )
            smallest = LEFT(i);
        /*else if ( LEFT(i) < openlist->size() && (openlist->at(i)->fForward == openlist->at(LEFT(i))->fForward) &&
                  ((openlist->at(i)->fForward - openlist->at(i)->gForward) >
                   (openlist->at(LEFT(i))->fForward - openlist->at(LEFT(i))->gForward)) )
            smallest = LEFT(i);*/
        else
            smallest = i;
        if ( RIGHT(i) < openlist->size() && (openlist->at(smallest)->fForward > openlist->at(RIGHT(i))->fForward) )
            smallest = RIGHT(i);
        /*else if ( RIGHT(i) < openlist->size() && (openlist->at(smallest)->fForward == openlist->at(RIGHT(i))->fForward) &&
                  ((openlist->at(smallest)->fForward - openlist->at(smallest)->gForward) >
                   (openlist->at(RIGHT(i))->fForward - openlist->at(RIGHT(i))->gForward)) )
            smallest = RIGHT(i);*/
        if (smallest != i)
        {
            openlist->swap(i, smallest);
            env->grid[openlist->at(smallest)->ptMe.x()][openlist->at(smallest)->ptMe.y()].heapIndexForward = smallest;
            env->grid[openlist->at(i)->ptMe.x()][openlist->at(i)->ptMe.y()].heapIndexForward = i;
            i = smallest;
        }
        else
            break;
    }
}

//@function     check if current exists in heap, return position        O(lg n)
int heapExistsKeyF(QList<POSITION*> *openlist, POSITION *current)
{
    if (!openlist->size())    // heap empty
        return -1;
    else
    {
        //while()
    }
}













//@function     insert current in openlist heap     2*O(lg n)
void minHeapInsertB(QList<POSITION*> *openlist, POSITION* current, Environment *env)
{
    //register int pos = heapExistsKey(openlist, current);    // O(lg n)
    int pos = env->grid[current->ptMe.x()][current->ptMe.y()].heapIndexBackward;
    if (pos > -1)       // if node already exist
        heapDecreaseKeyB(openlist, pos, env);     // O(lg n)
    else
    {
        openlist->append(current);      // O(lg n)
        env->grid[current->ptMe.x()][current->ptMe.y()].heapIndexBackward = openlist->size()-1;
        heapDecreaseKeyB(openlist, openlist->size()-1, env);      // O(lg n)
    }
}

//@function     decrease key of an element at position pos      O(lg n)
void heapDecreaseKeyB(QList<POSITION*> *openlist, int pos, Environment *env)
{
    MYINT key = openlist->at(pos)->fBackward;
    //env->grid[openlist->at(pos)->ptMe.x()][openlist->at(pos)->ptMe.y()].heapIndex = pos;
    for(; pos > 0 && (openlist->at(PARENT(pos))->fBackward > key); pos = PARENT(pos))
    {
        openlist->swap(pos, PARENT(pos));
        env->grid[openlist->at(PARENT(pos))->ptMe.x()][openlist->at(PARENT(pos))->ptMe.y()].heapIndexBackward = PARENT(pos);
        env->grid[openlist->at(pos)->ptMe.x()][openlist->at(pos)->ptMe.y()].heapIndexBackward = pos;
    }
}

//@function     build min heap      O(n)
void buildMinHeapB(QList<POSITION*> *openlist, Environment *env)
{
    if (openlist->size())
    {
        // let everyone know its index
        for (int i=0; i<openlist->size(); i++)
            env->grid[openlist->at(i)->ptMe.x()][openlist->at(i)->ptMe.y()].heapIndexBackward = i;

        for(int i = PARENT(openlist->size()-1); i>=0; --i)
            minHeapifyB(openlist, i, env);
    }
}

//@function     extract minimum     O(lg n)
POSITION* extractMinB(QList<POSITION*> *openlist, Environment *env)
{
    if (!openlist->size())
        return NULL;
    else
    {
        // not adjusting heapIndex of the node extracted, because it will not be used again in case of WA*
        openlist->swap(0, openlist->size()-1);      // swap first with last
        env->grid[openlist->at(0)->ptMe.x()][openlist->at(0)->ptMe.y()].heapIndexBackward = 0;
        POSITION *temp = openlist->takeLast();

        // minHeapify at position 0
        minHeapifyB(openlist, 0, env);
        env->grid[temp->ptMe.x()][temp->ptMe.y()].heapIndexBackward = -1;   // needed in case of ARA*
#ifdef DEBUG_HEAP
        qDebug("BH: (%d, %d)", temp->ptMe.x(), temp->ptMe.y());
#endif
        return temp;
    }
}

//@function     min Heapify openlist    O(lg n)
void minHeapifyB(QList<POSITION*> *openlist, int i, Environment *env)
{
    int smallest = -1;
    while(1)
    {
        //if (LEFT(i) < openlist->size() && openlist->at(LEFT(i))->fBackward < openlist->at(i)->fBackward)
        if ( LEFT(i) < openlist->size() && (openlist->at(i)->fBackward > openlist->at(LEFT(i))->fBackward) )
            smallest = LEFT(i);
        else
            smallest = i;
        //if (RIGHT(i) < openlist->size() && openlist->at(RIGHT(i))->fBackward < openlist->at(smallest)->fBackward)
        if ( RIGHT(i) < openlist->size() && (openlist->at(smallest)->fBackward > openlist->at(RIGHT(i))->fBackward) )
            smallest = RIGHT(i);
        if (smallest != i)
        {
            openlist->swap(i, smallest);
            env->grid[openlist->at(smallest)->ptMe.x()][openlist->at(smallest)->ptMe.y()].heapIndexBackward = smallest;
            env->grid[openlist->at(i)->ptMe.x()][openlist->at(i)->ptMe.y()].heapIndexBackward = i;
            i = smallest;
        }
        else
            break;
    }
}

//@function     check if current exists in heap, return position        O(lg n)
int heapExistsKeyB(QList<POSITION*> *openlist, POSITION *current)
{
    if (!openlist->size())    // heap empty
        return -1;
    else
    {
        //while()
    }
}









