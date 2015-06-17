#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "headers.h"

#define HCOST 1000  // for 1            *1000
#define DCOST 1414  // for sqrt(2)      *1000
#define DEFAULT_SEARCH_TYPE "WA*"

#define CAUTION         // Checks null returns
#define ARASTAR       // To use link for successor generation
#define LARGESIZE       // To turn off display
//#define DEBUG
//#define DEBUG_HEAP      // Only debugging the Heap
#define WRITEIMAGE          // To decide between writing image or drawing it on a window
#define MYASSERT        // making assertions to ensure correctness of A*
//#define MYASSERT_VALIDATE_HEAP  // heap validation

//#define CALLFROMCOMMANDLINE     // To turn of gui things in case of commandline calling
//#define STOPIMAGEFILECREATION   // stop image file creation in visaulSearch(WA*) when generating random blocked config files
//#define GENERATESEARCHSPACE     // To turn of printing of paths when generating random blocked config files

typedef qint64 MYINT;     // max - 4.2 billion

namespace searchEnv {
    struct pos;
    struct gridState;
    struct printInformation;
    class Environment;
}

typedef struct searchEnv::pos       // stores information pertaining to a state
{
    QPoint ptMe;

    struct searchEnv::pos *parentForward;
    MYINT fForward;
    MYINT gForward;
    //int fForward;
    //int gForward;

    struct searchEnv::pos *parentBackward;
    MYINT gBackward;
    MYINT fBackward;
    //int gBackward;
    //int fBackward;
    pos():
        fForward(0), gForward(0), fBackward(0), gBackward(0), parentForward(NULL), parentBackward(NULL) {}
} POSITION;

typedef struct searchEnv::gridState     // store infromation pertaining to the environment
{
    bool onPathCommon;      // worthless, obsolete
    bool onPathForward;     // worthless, obsolete
    bool onPathBackward;    // worthless, obsolete

    QPoint ptMe;
    bool pathExists;
    struct searchEnv::pos *link;

    bool visitedForward;
    int heapIndexForward;
    bool onClosedListForward;   // this closed list is not ARA* ImprovePath CLOSED List, it is BWA* closed list to check reexpansion
    //int onClosedListForwardCount;

    bool visitedBackward;
    int heapIndexBackward;
    bool onClosedListBackward;  // this closed list is not ARA* ImprovePath CLOSED List, it is BWA* closed list to check reexpansion
    //int onClosedListBackwardCount;
    gridState():
        link(NULL), onClosedListForward(false), onClosedListBackward(false), visitedForward(false), visitedBackward(false) {}
} GRIDSTATE;

class searchEnv::Environment
{
public:
    QPoint ptGridSize;    // considering the goal and start state as well, so both sizes incrased by a factor of 2
    GRIDSTATE **grid;       // the 2D grid
    QPoint ptStartForward, ptGoalForward;     // start and goal state coordinates
    QPoint ptStartBackward, ptGoalBackward;     // start and goal state coordinates for Backward search
    short obsthresh;

    int epsilon;
    MYINT numberOfNodesExpandedForward;
    MYINT numberOfNodesExpandedBackward;
    MYINT arastarTotalNodesExpanded;
    MYINT arastarTotalTimeTaken;
    Environment():
        numberOfNodesExpandedForward(0), numberOfNodesExpandedBackward(0), arastarTotalTimeTaken(0), arastarTotalNodesExpanded(0) {}
};

typedef struct searchEnv::printInformation     // Contains printing information
{
    MYINT solutionCost;
    MYINT runtime;
    MYINT noOfExpansions;
    printInformation():
        solutionCost(2000000000), runtime(2000000000), noOfExpansions(2000000000) {}
} PRINTINFORMATION;

#endif // STRUCTURES_H
