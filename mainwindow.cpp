
#include "headers.h"

QRgb globalColorValue = qRgb(255, 0, 0);    // Red in the beginning
bool globalChkZoomIN = false;
bool globalChkZoomOUT = false;
int globalZoomTimes = 2;
int MY_INFINITY = 2000000000;   // 2 billion

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // My stuff
    //ui->chkZoomOut->setEnabled(false);
    //ui->leZoomOut->setEnabled(false);
    ui->btnClearDrawing->setEnabled(false);
    ui->btnCompare->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}








using namespace searchEnv;


//tempbyme
void MainWindow::callMeFromCMD()
{
    //comapreSearchCInvertedCCL();
    //createRandomBlockedConfigurationsCCL();
    //comapreBlockbasedSearchCCL();
    //createLShapeRandomBlockedConfigurationsCCL();
    //createOutdoorBlockedConfigurationsCCL();
    comapreARAstarBlockbasedSearchCCL();
    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection); // very important line for terminating the entire application
    //QTextStream(stdout) << ">>>" + QCoreApplication::arguments().at(1) + " " + QCoreApplication::arguments().at(2) << endl;
}

void MainWindow::on_btnTestor_clicked()
{
    int **grid = AllocateDynamicArray<int>(4,3);
    for (int j=0; j<3; j++)
        for (int i=0; i<4; i++)
            grid[i][j]=i;
    //composeSearchSpaceFile(grid, 1, 0, 0, 89, 99);
    //testor();
    //genrateRandomSearchSpace();
    //mergeFiles();
    //createRandomBlockedConfigurations();
    //createInvertedCBlockedConfigurations();
    //createLShapeRandomBlockedConfigurationsCCL();
    //createOutdoorBlockedConfigurationsCCL();
    //findMeanStdDev();
    //comapreARAstarBlockbasedSearchCCL();return;

    QHash<QString, bool> testlist;
    for (int i=0; i<1000; i++)
        for (int j=0; j<1000; j++)
        {
            testlist.insert(QString::number(i)+"ab"+QString::number(j),true);
        }
    QList<QString> keyList = testlist.uniqueKeys();

    if ( keyList.size() < testlist.size() )
        qDebug() << "Stuck Up";
    return;

    int sz = testlist.size();
    for (int i=0; i<sz+1; i++)
    {
        if (testlist.value("a"+QString::number(i), false))
        {
            testlist.remove("a"+QString::number(i));
        }
    }
    return;

    int y = 62;
    qDebug() << (int)(0.1*y);
    if ((int)0.1*y > 5)
        qDebug() << "yes";
    else
        qDebug() << "no";
    return;

    float x = y*1.9f + 0.8f;
    qDebug() << x;
    qDebug() << "int " << sizeof(int) << " long int " << sizeof(long int) << " uint " << sizeof(unsigned int);
    float f1 = 3.202f, f2 = 3.2023f;
    qDebug() << compareFloatValues(f1,f2);
    //bool str = f1>f2?true:false;
    bool str = MY_INFINITY>MY_INFINITY-10000?true:false;
    qDebug() << str;
    int infint = MY_INFINITY;
    qDebug() << infint;


    /*QPropertyAnimation animation(ui->btnCompare, "geometry");
    animation.setDuration(10000);
    animation.setStartValue(QRect(0, 0, 100, 30));
    animation.setEndValue(QRect(250, 250, 250, 500));

    animation.start();*/
}

//@function     sort QPoints increasing order
void sortQPoint(QList<QPoint> *closedlist)
{
    for (int i=0; i< closedlist->size(); i++)
    {
        for(int j=i+1; j< closedlist->size(); j++)
        {
            if (closedlist->at(i).x()>closedlist->at(j).x())
                closedlist->swap(i,j);
            else if ((closedlist->at(i).x()==closedlist->at(j).x()) && closedlist->at(i).y()>closedlist->at(j).y())
                closedlist->swap(i,j);
        }
    }

    QFile file("sortlist");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Error file opening the config file for sorting list, Exiting...";
        return;
    }
    QTextStream outt(&file);
    QString str;
    for (int i =0; i< closedlist->size(); i++)
    {
        str = QString::number(closedlist->at(i).x()) + "  " + QString::number(closedlist->at(i).y());
        outt << str;
    }
    file.close();
}

//@function     prepares the environment from the config file, for the search to proceed
int prepareEnvironment(QString configFile, Environment& env)
{
    // we have not done rigrious exception handling, we leave onous error reduction on user
    QFile file(configFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error file opening the config file, Exiting...";
        return -1;
    }

    QTextStream in(&file);
    QStringList spaceSplitted;
    QRegularExpression respace(" ");
    bool ok;

    // read line1 >> gridsizeX
    QString line = in.readLine();
    spaceSplitted = line.split(respace);
    env.ptGridSize.setX(spaceSplitted.at(1).toInt(&ok));
    env.ptGridSize.setY(spaceSplitted.at(2).toInt(&ok));

    // read line2 >> obsthresh
    line = in.readLine();
    spaceSplitted = line.split(respace);
    env.obsthresh = spaceSplitted.at(1).toInt(&ok);

    // read line3 >> startstate
    line = in.readLine();
    spaceSplitted = line.split(respace);
    env.ptStartForward.setX(spaceSplitted.at(1).toInt(&ok));
    env.ptStartForward.setY(spaceSplitted.at(2).toInt(&ok));
    //glb.goalDir = commaSplitted.at(3).at(0).toLatin1();

    // read line4 >> goalstate
    line = in.readLine();
    spaceSplitted = line.split(respace);
    env.ptGoalForward.setX(spaceSplitted.at(1).toInt(&ok));
    env.ptGoalForward.setY(spaceSplitted.at(2).toInt(&ok));
    //glb.goalDir = commaSplitted.at(3).at(0).toLatin1();

    // read line5 environement
    line = in.readLine();

    // now plot the grid
    env.grid = AllocateDynamicArray<GRIDSTATE>(env.ptGridSize.x(), env.ptGridSize.y());

    // set interior states
    for(int j=0; j<env.ptGridSize.y(); j++)   // increment y
    {
        line = in.readLine();
        if (line.isEmpty())
        {
            qDebug() << "error in configuration file.";
            break;
        }

        spaceSplitted = line.split(respace);
        int k = 0;
        for(int i=0;k<env.ptGridSize.x();i++)
        {
            if(!spaceSplitted.at(i).isEmpty())
            {
                //qDebug() << spaceSplitted.at(i).toInt(&ok);
                env.grid[k][j].pathExists = !(spaceSplitted.at(i).toInt(&ok));
                env.grid[k][j].ptMe.setX(k);
                env.grid[k][j].ptMe.setY(j);
                env.grid[k][j].visitedForward = false;
                env.grid[k][j].heapIndexForward = -1;
                env.grid[k][j].onPathForward = false;
                env.grid[k][j].onClosedListForward = false;
                env.grid[k][j].visitedBackward = false;
                env.grid[k][j].heapIndexBackward = -1;
                env.grid[k][j].onPathBackward = false;
                env.grid[k][j].onClosedListBackward = false;
                env.grid[k][j].link = NULL;
                env.grid[k][j].onPathCommon = false;
                k++;                                // increment x
            }
        }
        k=0;
    }
    file.close();
    return 0;
}

void MainWindow::on_btnSearch_clicked()
{
    bool ok;
    // search global variable
    Environment env;
    QString configFile = ui->lePath->text();  // read env file
    // initialize data structures: STACK, HEAP as per the searchType requirement
    // plot the environment and globals
    if (prepareEnvironment(configFile, env))
    {
        qDebug() << "Error in prepareEnvironment, Exiting...";
        return;
    }

    // determine search type
    QString searchType = DEFAULT_SEARCH_TYPE;
    if (ui->leSearchType->text()=="WA*")
        searchType = "WA*";
    else if (ui->leSearchType->text()=="ARA*" || ui->chkARAstar->isChecked())
        searchType = "ARA*";
    else if (ui->leSearchType->text()=="BWA*" || ui->chkBidirastar->isChecked())
        searchType = "BWA*";
    else if (ui->chko1BidirARAstar->isChecked())
        searchType = "o1BARA*";
    else if (ui->chko2BidirARAstar->isChecked())
        searchType = "o2BARA*";
    else if (ui->chksBWAstar->isChecked())
        searchType = "sBWA*";
    else if (ui->chkrsBWAstar->isChecked())
        searchType = "rsBWA*";
    else if (ui->chkkorfBWAstar->isChecked())
        searchType = "korfBWA*";

    // set start and goal state
    if (!ui->leStart->text().isEmpty())
    {
        QStringList spaceSplitted;
        QRegularExpression respace(" ");
        bool ok;
        spaceSplitted = ui->leStart->text().split(respace);
        for (int i=0; i<spaceSplitted.size();)
            if (spaceSplitted.at(i).isEmpty())
                spaceSplitted.removeAt(i);
            else
                i++;
        env.ptStartForward.setX(spaceSplitted.at(0).toInt(&ok));
        env.ptStartForward.setY(spaceSplitted.at(1).toInt(&ok));
    }
    if (!ui->leGoal->text().isEmpty())
    {
        QStringList spaceSplitted;
        QRegularExpression respace(" ");
        bool ok;
        for (int i=0; i<spaceSplitted.size();)
            if (spaceSplitted.at(i).isEmpty())
                spaceSplitted.removeAt(i);
            else
                i++;
        spaceSplitted = ui->leGoal->text().split(respace);
        env.ptGoalForward.setX(spaceSplitted.at(0).toInt(&ok));
        env.ptGoalForward.setY(spaceSplitted.at(1).toInt(&ok));
    }

    // swap start and goal state
    if (ui->chkSwap->isChecked())
    {
        int tempX = env.ptStartForward.x(), tempY = env.ptStartForward.y();
        env.ptStartForward.setX(env.ptGoalForward.x()); env.ptStartForward.setY(env.ptGoalForward.y());
        env.ptGoalForward.setX(tempX); env.ptGoalForward.setY(tempY);
    }

    globalChkZoomIN = false;
    globalChkZoomOUT = false;
    globalZoomTimes = 1;    // Default value
    if (ui->chkZoomIn->isChecked())
    {
        globalChkZoomIN = true;
        if (!ui->leZoomIn->text().isEmpty())
            globalZoomTimes = ui->leZoomIn->text().toInt(&ok);
    }
    else if (ui->chkZoomOut->isChecked())
    {
        globalChkZoomOUT = true;
        if (!ui->leZoomOut->text().isEmpty())
            globalZoomTimes = ui->leZoomOut->text().toInt(&ok);
    }

    // Set start and goal for backward
    env.ptStartBackward = env.ptGoalForward;
    env.ptGoalBackward = env.ptStartForward;

    env.arastarTotalNodesExpanded = 0;
    // draw label grid
    //MainWindow::guiHandler(&env);

    // call visual search
    int retval = visualSearch(searchType, &env);
    //QDebug() << "Running "
    if (!retval)
    {
        qDebug() << "Search was successful! :o Exiting...";
        if ( searchType=="WA*" || searchType=="ARA*" )
            if ( checkPathCorrectness(&env, env.grid[env.ptStartForward.x()][env.ptStartForward.y()].link,
                                      env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].link, true) )
                qDebug() << "Path is VALID";
            else
                qDebug() << "Path is INVALID";
    }
    else
        qDebug() << "Search was unsuccessful!! :(  Exiting...";

    qDebug() << searchType << "completed.";

    // Free heap memory
    for(int j=0; j<env.ptGridSize.y(); j++)
        for(int i=0; i<env.ptGridSize.x(); i++)
            delete env.grid[i][j].link;

    FreeDynamicArray(env.grid);
}

void printARAstarPath(POSITION *goal, Environment* env, QTime *t)
{
    QString timeElapsed = QString::number(t->elapsed());
    qDebug() << "printing " + QString::number(env->epsilon) + " bounded path------------------------";
    POSITION *temp = goal;
    int pathlength = 0;
    while(temp->ptMe != env->ptStartForward)
    {
        env->grid[temp->ptMe.x()][temp->ptMe.y()].onPathForward = true;
        qDebug() << temp->ptMe;
        temp = temp->parentForward;
        pathlength++;
    }
    env->grid[temp->ptMe.x()][temp->ptMe.y()].onPathForward = true;     // for start state
    qDebug() << temp->ptMe;

    qDebug() << "eps: " + QString::number(env->epsilon) + " Path cost     " + QString::number(goal->gForward/1000); // normalizing the cost
    qDebug() << "eps: " + QString::number(env->epsilon) + " Expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward);
    qDebug() << "eps: " + QString::number(env->epsilon) + " Elapsed time = " + timeElapsed;
    //qDebug() << "Total expanded nodes:  " + QString::number(env->arastarTotalNodesExpanded);
}

void printBidirAstarPath(POSITION *current, Environment* env, QTime *t, bool common)     // current is the meeting point
{
    static int printSequence;
    printSequence++;
    QString timeElapsed = QString::number(t->elapsed());
    int epsilon = env->epsilon;

    qDebug() << "printing " + QString::number(epsilon) + " bounded path------------------------";
    POSITION *temp = current;
    env->grid[current->ptMe.x()][current->ptMe.y()].onPathCommon = true;
    QLinkedList<POSITION*> pathList;      // Start to Goal LinkedList
    int pathlength = 1;     // common node included
    while(temp->parentForward)             // for Forward
    {
        temp = temp->parentForward;
        pathList.prepend(temp);
        env->grid[temp->ptMe.x()][temp->ptMe.y()].onPathForward = true;
        pathlength++;
    }
    pathList.append(current);
    temp = current;
    while(temp->parentBackward)             // for Backward
    {
        temp = temp->parentBackward;
        pathList.append(temp);
        env->grid[temp->ptMe.x()][temp->ptMe.y()].onPathForward = true;
        pathlength++;
    }
    while(pathList.size())
        qDebug() << pathList.takeLast()->ptMe;

    qDebug() << "--common--";
    qDebug() << current->ptMe;
    qDebug() << "eps: " + QString::number(epsilon);
    qDebug() << "Solution Number: " + QString::number(printSequence);
    qDebug() << "Path cost =   " + QString::number((current->gForward + current->gBackward)/1000);  // normalizing the cost
    qDebug() << "Forward expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward);
    qDebug() << "Backward expanded nodes:  " + QString::number(env->numberOfNodesExpandedBackward);
    qDebug() << "Total expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward + env->numberOfNodesExpandedBackward);
    qDebug() << "Elapsed time =   " + timeElapsed;


    // conditional handling of common
    /*
    if (common)     // Assumption: current cannot be start or goal node
    {
        qDebug() << "printing " + QString::number(epsilon) + " bounded path------------------------";
        POSITION *temp = current;
        env->grid[current->ptMe.x()][current->ptMe.y()].onPathCommon = true;
        QLinkedList<POSITION*> pathList;      // Start to Goal LinkedList
        int pathlength = 1;     // common node included
        while(temp)             // for Forward
        {
            temp = temp->parentForward;
            pathList.prepend(temp);
            env->grid[temp->ptMe.x()][temp->ptMe.y()].onPathForward = true;
            pathlength++;
        }
        pathList.append(current);
        temp = current;
        while(temp)             // for Backward
        {
            temp = temp->parentBackward;
            pathList.append(temp);
            env->grid[temp->ptMe.x()][temp->ptMe.y()].onPathForward = true;
            pathlength++;
        }

        while(pathList.size())
            qDebug() << pathList.takeLast()->ptMe;

        qDebug() << "eps: " + QString::number(epsilon) + " Path cost =   " + QString::number(pathlength);
        qDebug() << "eps: " + QString::number(epsilon) + " Forward expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward);
        qDebug() << "eps: " + QString::number(epsilon) + " Backward expanded nodes:  " + QString::number(env->numberOfNodesExpandedBackward);
        qDebug() << "eps: " + QString::number(epsilon) + " Total expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward + env->numberOfNodesExpandedBackward);
        qDebug() << "eps: " + QString::number(epsilon) + " Elapsed time =   " + timeElapsed;

#ifndef LARGESIZE
                newGuiHandler(env, &epsilon);
#endif
    }
    else
    {
        if (current->ptMe = env->ptGoalForward)     // case forward goal reached
        {

        }
        else            // case backward goal reached
        {

        }
    }
*/

}

//@function Write image file
void composeImageFile(QImage *image, bool lastRound, QString _imageFilePath = "", int epsilon = 1)
{
#define MAX_HIT_LIMIT 50000
    static int maxHit = MAX_HIT_LIMIT + 2;  // set it true for first call
    static int fileNumber = 1;

    QString path;
    if ( _imageFilePath == "")
        path = "/home/peace/testResults/";
    else
        path = _imageFilePath;

    QString imageFilePath = path + "trace" + QString::number(fileNumber) + ".png";

    if ( maxHit > (epsilon * MAX_HIT_LIMIT) || lastRound)
        //if ( maxHit > (-1) || lastRound)
    {
        QImageWriter writer( imageFilePath, "png" );
        writer.setText("Author", "HB");
        if (globalChkZoomIN)
            writer.write( composeZoomedINimageFile(image) );
        else if (globalChkZoomOUT)
            writer.write( composeZoomedOUTimageFile(image) );
        else
            writer.write(*image);
        maxHit = 0;
        fileNumber++;
        // Toggle color values
        globalColorValue == qRgb(255, 0, 0) ? globalColorValue = qRgb(0, 0, 255) : globalColorValue = qRgb(255, 0, 0);
    }
    else
        maxHit++;
}

//@function     create QImage for newly generated states
void perPixelGridPainter(QImage *image, QPoint *ptMe, bool lastRound, bool direction = false, QString imageFilePath = "", int epsilon = 1)
{
    QRgb value;
    if (direction)   // forward
        //value = qRgb(255, 0, 0); // Red
        value = globalColorValue;
    else        // backward
        value = qRgb(0, 0, 255);
    if (!ptMe)
        qDebug() << ptMe;
    image->setPixel(*ptMe, value);
    composeImageFile(image, lastRound, imageFilePath, epsilon);
}

//@function     create QImage with intial config file
void oneTimeGridPainter(QImage *image, GRIDSTATE **displayGrid, int IMAGE_X, int IMAGE_Y, int startX,
                        int startY, int goalX, int goalY, QString imageFilePath = "")
{

    //int IMAGE_X = 1000, IMAGE_Y = 1000;
    //QImage image(IMAGE_Y, IMAGE_X, QImage::Format_RGB32);
    QRgb value;

    // FOR THE ORDERING
    // common (green)
    // goal + start (black)
    // blocked (red)

    for( int j=0; j<IMAGE_Y; j++ )
    {
        for( int i=0; i<IMAGE_X; i++ )
        {
            if ( displayGrid[i][j].pathExists )
            {
                if ( i==startX && j==startY || i==goalX && j==goalY )       // start or goal states
                {
                    value = qRgb(0, 0, 0); // black
                    image->setPixel(i, j, value);
                }
                else
                {
                    value = qRgb(0, 250, 0); // green
                    image->setPixel(i, j, value);
                }
            }
            else    // Obstacle blocks
            {
                //value = qRgb(250, 0, 0); // red
                value = qRgb(250, 246, 2); // yellow
                image->setPixel(i, j, value);
            }
        }
    }

    // darken the start and goal nodes
    if (!globalChkZoomIN && !globalChkZoomOUT)
    {
        value = qRgb(0, 0, 0); // black
        image->setPixel(startX, startY, value);
        image->setPixel(startX+1, startY, value);
        image->setPixel(startX, startY+1, value);
        image->setPixel(startX+1, startY+1, value);
        image->setPixel(startX+2, startY, value);
        image->setPixel(startX+2, startY+1, value);
        image->setPixel(startX+2, startY+2, value);
        image->setPixel(startX+1, startY+2, value);
        image->setPixel(startX, startY+2, value);
        image->setPixel(goalX, goalY, value);
        image->setPixel(goalX+1, goalY, value);
        image->setPixel(goalX, goalY+1, value);
        image->setPixel(goalX+1, goalY+1, value);
        image->setPixel(goalX+2, goalY, value);
        image->setPixel(goalX+2, goalY+1, value);
        image->setPixel(goalX+2, goalY+2, value);
        image->setPixel(goalX+1, goalY+2, value);
        image->setPixel(goalX, goalY+2, value);
    }

    composeImageFile(image, false, imageFilePath);
}

//@function     search
int MainWindow::visualSearch(QString searchType, Environment *env)
{
#define MIN_EPSILON 1       // for ARA* family
    if (searchType == "WA*")
    {
        qDebug() << "==============================WA*============================="; // norwal weighted A*
        QTime t;
        t.start();

        int epsilon = 1;      // weight
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }
        env->epsilon = epsilon;         // for debugging heap

        POSITION *child = NULL;

        //POSITION *current = &env.grid[env->ptStart.x()][env->ptStart.y()];     // start node
        POSITION *current = new POSITION;    // start node
        current->ptMe = env->ptStartForward;

        QList<POSITION *> openlist;    // priority Queue
        current->gForward = 0;
        env->grid[current->ptMe.x()][current->ptMe.y()].visitedForward = true;
        env->grid[current->ptMe.x()][current->ptMe.y()].link = current;
        current->fForward = current->gForward + epsilon * hvalueForward(current, env);
        current->parentForward = NULL;
        minHeapInsertF(&openlist, current, env);
        MYINT numberOfNodesExpanded = 0;

        // Draw one time image
        QImage *image;
        if (!globalChkZoomIN && !globalChkZoomOUT)
            image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
        else
            image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid

        oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());
        //thread()->msleep(5000);

        MYINT previousFvalue = 0;
        QFile Anomalyfile("/home/peace/testResults/anomaly.txt");
        if (!Anomalyfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return 1;
        }
        QTextStream out1(&Anomalyfile);

        while(current->ptMe != env->ptGoalForward)  // while(goal node is not expanded)
        {
#ifdef MYASSERT_VALIDATE_HEAP
            validateHeap(&openlist);     // heap debug [12-June-15]
#endif
#ifdef CAUTION
            if (!(current = extractMinF(&openlist, env)))   // take the first node from the OPEN
            {
                composeImageFile(image, true);
                qDebug() << "FAILURE: openlist returned NULL";
                return 1;
            }
#else
            current = extractMinF(&openlist, env);   // take the first node from the OPEN
#endif

#ifdef MYASSERT
            if ( previousFvalue > current->fForward && epsilon == 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "previoiusFvalue = " << previousFvalue << "  currentFvalue = " << current->fForward;
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpanded);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalue = current->fForward;
#endif
            /*if (numberOfNodesExpanded == 81250)
                qDebug() << "yes";*/

            // Stopping reexpansion of nodes
            if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                continue;
            else
                env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

            //expandlist.push_back(current->ptMe);  // temp
            numberOfNodesExpanded++;
#ifdef DEBUG
            out1 << "---------------" << endl;
            out1 << "f= " << current->fForward << "     g= " << current->gForward << "  h= " << current->fForward-current->gForward << endl;
            out1 << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y()) << endl;
#endif
            //qDebug() << "point " << current->ptMe << " fvalue " << current->fForward;

            // Paint the current pixel
#ifndef STOPIMAGEFILECREATION
            perPixelGridPainter(image, &(current->ptMe), false, true);
#endif
            //thread()->msleep(100);

            // print current
            //qDebug() << "----------------------------";
            //qDebug() << current->ptMe;

            if (current->ptMe == env->ptGoalForward)    // goal expanded
            {
                //qDebug() << "No of nodes expanded:  " + QString::number(numberOfNodesExpanded);
                env->numberOfNodesExpandedForward = numberOfNodesExpanded;
                env->epsilon = epsilon;
                //sortQPoint(&expandlist);
#ifndef GENERATESEARCHSPACE
                printARAstarPath(current, env, &t);
#endif
#ifndef STOPIMAGEFILECREATION
                perPixelGridPainter(image, &(current->ptMe), true, false);
#endif
                out1.flush();
                Anomalyfile.close();
                return 0;   // success
            }

            QVector<POSITION*> pVect;     // vector of children of current
            findChildren_(&pVect, current, env);

            if (!pVect.size())
                continue;
            else        // set ptParent for each child
            {
                while(pVect.size())
                {
                    child = pVect.takeLast();
                    if ( !env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward )
                        child->fForward = child->gForward = MY_INFINITY;
                    if ( child->gForward > current->gForward + cost(current, child) )
                    {
                        child->gForward = current->gForward + cost(current, child);
                        child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                        minHeapInsertF(&openlist, child, env);
                        child->parentForward = current;
                        env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;
                        // single assert
                        if ( child->fForward < current->fForward && epsilon == 1 )
                        {
                            qDebug() << "Great error, child-current f-value comparision... Terminating...";
                            return 1;
                        }
                    }
                }
            }
            //QThread::sleep(1);
        }
        out1.flush();
        Anomalyfile.close();
        return 1;   // failure
    }
    else if (searchType == "ARA*")
    {
        qDebug() << "==============================ARA*=============================";    // normal ARA*
        bool ok;

        int epsilon = 5;      // weight
        int decrement = 1;

        if (!ui->leEpsilon->text().isEmpty())
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        if (!ui->leDecrement->text().isEmpty())
            decrement = ui->leDecrement->text().toInt(&ok);   // decrement
        env->epsilon = epsilon;         // for debugging heap

        QList<POSITION *> openlist;    // priority Queue
        QHash<QString, POSITION*> inconslist;

        POSITION *goal = new POSITION;
        goal->ptMe = env->ptGoalForward;
        goal->parentForward = NULL;
        goal->gForward = MY_INFINITY;
        goal->fForward = fvalueForward(goal, env, &epsilon);
        env->grid[goal->ptMe.x()][goal->ptMe.y()].link = goal;

        POSITION *current = new POSITION;    // start node
        current->ptMe = env->ptStartForward;
        current->gForward = 0;
        env->grid[current->ptMe.x()][current->ptMe.y()].visitedForward = true;
        env->grid[current->ptMe.x()][current->ptMe.y()].link = current;
        current->fForward = fvalueForward(current, env, &epsilon);
        current->parentForward = NULL;
        minHeapInsertF(&openlist, current, env);

#ifdef CAUTION
        if (improvePath(goal, epsilon, &openlist, &inconslist, env))
        {
            qDebug() << "FAILURE: returning from improvePath, before while()";
            return 1;
        }
#else
        improvePath(goal, epsilon, &openlist, &inconslist, env);   // also publishes the solution
#endif

        //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )

        while(epsilon > MIN_EPSILON)  // while(goal node is not expanded)
        {
            epsilon = epsilon - decrement;
            env->epsilon = epsilon;         // for debugging heap

            // merge openlist with inconslist
            QList<QString> keyList = inconslist.uniqueKeys();
#ifdef MYASSERT     // heap debug [12-June-15]
            if ( inconslist.size()!=keyList.size() )
            {
                qDebug() << "FAILURE: meging openlist with inconslist, there is some repeatition in inconslist, abrupt termination";
                return 1;
            }
#endif
            for (int i=0; i<keyList.size(); i++)
            {
                env->grid[inconslist.value(keyList.at(i))->ptMe.x()][inconslist.value(keyList.at(i))->ptMe.y()].onClosedListForward = false;
                openlist.append(inconslist.value(keyList.at(i)));
                inconslist.remove(keyList.at(i));
            }
            for (int i=0; i<openlist.size(); i++)     // update f values for new epsilon
                openlist.at(i)->fForward = fvalueForward(openlist.at(i), env, &epsilon);

            /*for (int i =0; i<openlist.size(); i++)     // sort
                for (int j=i+1; j<openlist.size(); j++)
                    if (openlist.at(i)->f > openlist.at(j)->f)
                        openlist.swap(i,j);
            for (int i =0; i<inconslist.size(); i++)     // sort
                for (int j=i+1; j<inconslist.size(); j++)
                    if (inconslist.at(i)->f > inconslist.at(j)->f)
                        inconslist.swap(i,j);

            for (int i=0,j=0; i<inconslist.size();)    // merge
            {
                while(inconslist.at(i)->f > openlist.at(j)->f)
                    j++;
                while(inconslist.at(i)->f <= openlist.at(j)->f)
                {
                    openlist.insert(j, inconslist.at(i));
                    i++;
                }
            }*/
            buildMinHeapF(&openlist, env);
            //inconslist.clear();
#ifdef MYASSERT_VALIDATE_HEAP     // heap debug [12-June-15]
            if ( validateHeap(&openlist) )     // either heap inconstent or inconslist has some node
            {
                qDebug() << "FAILURE: returning from validateHeap in ARA* main(), abrupt termination";
                return 1;
            }
#endif

            qDebug() << "=================next iteration================";

            if (improvePath(goal, epsilon, &openlist, &inconslist, env))
            {
                qDebug() << "FAILURE: returning from improvePath";
                return 1;
            }

            //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
        }
        qDebug() << "----*----";
        qDebug() << "Total exp nodes " << env->arastarTotalNodesExpanded;
        return 0;   // success
    }
    else if (searchType == "o1BARA*")
    {
        qDebug() << "==============================BARA*=============================";   // bi-ARA* with optimization 1
        bool ok;

        int epsilon = 5;      // weight
        int decrement = 1;

        if (!ui->leEpsilon->text().isEmpty())
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        if (!ui->leDecrement->text().isEmpty())
            decrement = ui->leDecrement->text().toInt(&ok);   // decrement
        env->epsilon = epsilon;         // for debugging heap

        QList<POSITION *> openlistForward;    // priority Queue
        QHash<QString, POSITION*> inconslistForward;

        /*POSITION *goalForward = new POSITION;
        goalForward->ptMe = env->ptGoalForward;
        goalForward->parentForward = NULL;
        goalForward->gForward = MY_INFINITY;    // single most important line
        goalForward->fForward = fvalueForward(goalForward, env, &epsilon);
        env->grid[goalForward->ptMe.x()][goalForward->ptMe.y()].link = goalForward;*/

        POSITION *currentForward = new POSITION;    // start node
        currentForward->ptMe = env->ptStartForward;
        currentForward->gForward = 0;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].visitedForward = true;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].link = currentForward;
        currentForward->fForward = fvalueForward(currentForward, env, &epsilon);
        currentForward->parentForward = NULL;
        minHeapInsertF(&openlistForward, currentForward, env);

        QList<POSITION *> openlistBackward;    // priority Queue
        QHash<QString, POSITION*> inconslistBackward;

        /*POSITION *goalBackward = new POSITION;
        goalBackward->ptMe = env->ptGoalBackward;
        goalBackward->parentBackward = NULL;
        goalBackward->gBackward = MY_INFINITY;
        goalBackward->fBackward = fvalueBackward(goalBackward, env, &epsilon);
        env->grid[goalBackward->ptMe.x()][goalBackward->ptMe.y()].link = goalBackward;*/

        POSITION *currentBackward = new POSITION;    // start node
        currentBackward->ptMe = env->ptStartBackward;
        currentBackward->gBackward = 0;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].visitedBackward = true;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].link = currentBackward;
        currentBackward->fBackward = fvalueBackward(currentBackward, env, &epsilon);
        currentBackward->parentBackward = NULL;
        minHeapInsertB(&openlistBackward, currentBackward, env);

#ifdef CAUTION
        if (improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env))
        {
            qDebug() << "FAILURE: returning from improvePath, before while()";
            return 1;
        }
#else
        improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env);   // also publishes the solution
#endif

        //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )

        while(epsilon > MIN_EPSILON)  // while(goal node is not expanded)
        {
            epsilon = epsilon - decrement;
            env->epsilon = epsilon;         // for debugging heap

            // ---------FORWARD MERGE----------//
            // merge openlist with inconslistForward
            QHash<QString, POSITION*>::iterator inconsItrForward;
            for (inconsItrForward = inconslistForward.begin(); inconsItrForward != inconslistForward.end(); inconsItrForward++)
            {
                // WITH OPTIMIZATION 2
                if ( inconsItrForward.value() && !(inconsItrForward.value()->gForward < epsilon*hvalueBackward(inconsItrForward.value(), env)) )
                    openlistForward.append(inconsItrForward.value());
            }
            // update f values for new epsilon in inconslistForward
            for (int i =0; i<openlistForward.size(); i++)
                openlistForward.at(i)->fForward = fvalueForward(openlistForward.at(i), env, &epsilon);
            buildMinHeapF(&openlistForward, env);
            inconslistForward.clear();

            // ---------BACKWARD MERGE----------//
            // merge openlist with inconslistForward
            QHash<QString, POSITION*>::iterator inconsItrBackward;
            for (inconsItrBackward = inconslistBackward.begin(); inconsItrBackward != inconslistBackward.end(); inconsItrBackward++)
            {
                // WITH OPTIMIZATION 2
                if ( inconsItrBackward.value() && !(inconsItrBackward.value()->gBackward < epsilon*hvalueForward(inconsItrBackward.value(), env)) )
                    openlistBackward.append(inconsItrBackward.value());
            }
            // update f values for new epsilon in inconslistForward
            for (int i =0; i<openlistBackward.size(); i++)
                openlistBackward.at(i)->fBackward = fvalueBackward(openlistBackward.at(i), env, &epsilon);
            buildMinHeapB(&openlistBackward, env);
            inconslistBackward.clear();

            qDebug() << "=================next iteration================";
#ifdef CAUTION
            if (improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env))
            {
                qDebug() << "FAILURE: returning from improvePath";
                return 1;
            }
#else
            improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env);
#endif

            //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
        }
        qDebug() << "----*----";
        qDebug() << "Total exp nodes " << env->arastarTotalNodesExpanded;
        return 0;   // success
    }
    else if (searchType == "o2BARA*")
    {
        qDebug() << "==============================BARA*=============================";   // bi-ARA* with optimization 1 and 2
        bool ok;

        int epsilon = 5;      // weight
        int decrement = 1;

        if (!ui->leEpsilon->text().isEmpty())
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        if (!ui->leDecrement->text().isEmpty())
            decrement = ui->leDecrement->text().toInt(&ok);   // decrement
        env->epsilon = epsilon;         // for debugging heap

        QList<POSITION *> openlistForward;    // priority Queue
        QHash<QString, POSITION*> inconslistForward;

        /*POSITION *goalForward = new POSITION;
        goalForward->ptMe = env->ptGoalForward;
        goalForward->parentForward = NULL;
        goalForward->gForward = MY_INFINITY;    // single most important line
        goalForward->fForward = fvalueForward(goalForward, env, &epsilon);
        env->grid[goalForward->ptMe.x()][goalForward->ptMe.y()].link = goalForward;*/

        POSITION *currentForward = new POSITION;    // start node
        currentForward->ptMe = env->ptStartForward;
        currentForward->gForward = 0;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].visitedForward = true;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].link = currentForward;
        currentForward->fForward = fvalueForward(currentForward, env, &epsilon);
        currentForward->parentForward = NULL;
        minHeapInsertF(&openlistForward, currentForward, env);

        QList<POSITION *> openlistBackward;    // priority Queue
        QHash<QString, POSITION*> inconslistBackward;

        /*POSITION *goalBackward = new POSITION;
        goalBackward->ptMe = env->ptGoalBackward;
        goalBackward->parentBackward = NULL;
        goalBackward->gBackward = MY_INFINITY;
        goalBackward->fBackward = fvalueBackward(goalBackward, env, &epsilon);
        env->grid[goalBackward->ptMe.x()][goalBackward->ptMe.y()].link = goalBackward;*/

        POSITION *currentBackward = new POSITION;    // start node
        currentBackward->ptMe = env->ptStartBackward;
        currentBackward->gBackward = 0;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].visitedBackward = true;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].link = currentBackward;
        currentBackward->fBackward = fvalueBackward(currentBackward, env, &epsilon);
        currentBackward->parentBackward = NULL;
        minHeapInsertB(&openlistBackward, currentBackward, env);

#ifdef CAUTION
        if (improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env))
        {
            qDebug() << "FAILURE: returning from improvePath, before while()";
            return 1;
        }
#else
        improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env);   // also publishes the solution
#endif

        //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )

        while(epsilon > MIN_EPSILON)  // while(goal node is not expanded)
        {
            epsilon = epsilon - decrement;
            env->epsilon = epsilon;         // for debugging heap

            // ---------FORWARD MERGE----------//
            // merge openlist with inconslistForward
            QHash<QString, POSITION*>::iterator inconsItrForward;
            for (inconsItrForward = inconslistForward.begin(); inconsItrForward != inconslistForward.end(); inconsItrForward++)
            {
                // WITH OPTIMIZATION 2
                if ( inconsItrForward.value() && !(inconsItrForward.value()->gForward < epsilon*hvalueBackward(inconsItrForward.value(), env)) )
                    openlistForward.append(inconsItrForward.value());
            }
            // update f values for new epsilon in inconslistForward
            for (int i =0; i<openlistForward.size(); i++)
                openlistForward.at(i)->fForward = fvalueForward(openlistForward.at(i), env, &epsilon);
            buildMinHeapF(&openlistForward, env);
            inconslistForward.clear();

            // ---------BACKWARD MERGE----------//
            // merge openlist with inconslistForward
            QHash<QString, POSITION*>::iterator inconsItrBackward;
            for (inconsItrBackward = inconslistBackward.begin(); inconsItrBackward != inconslistBackward.end(); inconsItrBackward++)
            {
                // WITH OPTIMIZATION 2
                if ( inconsItrBackward.value() && !(inconsItrBackward.value()->gBackward < epsilon*hvalueForward(inconsItrBackward.value(), env)) )
                    openlistBackward.append(inconsItrBackward.value());
            }
            // update f values for new epsilon in inconslistForward
            for (int i =0; i<openlistBackward.size(); i++)
                openlistBackward.at(i)->fBackward = fvalueBackward(openlistBackward.at(i), env, &epsilon);
            buildMinHeapB(&openlistBackward, env);
            inconslistBackward.clear();

            qDebug() << "=================next iteration================";
#ifdef CAUTION
            if (improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env))
            {
                qDebug() << "FAILURE: returning from improvePath";
                return 1;
            }
#else
            improvePathBidir(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env);
#endif

            //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
        }
        qDebug() << "----*----";
        qDebug() << "Total exp nodes " << env->arastarTotalNodesExpanded;
        return 0;   // success
    }
    else if (searchType == "BWA*")
    {
        qDebug() << "==============================BWA*=============================";    // normal bi-WA*
        int incumbentSolutionCount = 0;
        QTime t;
        t.start();

        MYINT costIncumbentSolution = MY_INFINITY;

        int epsilon = 1;      // weight
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }
        env->epsilon = epsilon;         // for debugging heap

        POSITION *child = NULL, *current = NULL;

        POSITION *currentForward = new POSITION;    // Forward start node
        currentForward->ptMe = env->ptStartForward;
        currentForward->gForward = 0;
        currentForward->fForward = currentForward->gForward + epsilon * hvalueForward(currentForward, env);
        currentForward->parentForward = NULL;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].visitedForward = true;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].link = currentForward;
        QList<POSITION *> openlistForward;    // Forward priority Queue
        minHeapInsertF(&openlistForward, currentForward, env);
        int numberOfNodesExpandedForward = 0;
        //currentForward->fBackward = MY_INFINITY;
        //currentForward->gBackward = MY_INFINITY;

        POSITION *currentBackward = new POSITION;    // Backward start node
        currentBackward->ptMe = env->ptStartBackward;
        currentBackward->gBackward = 0;
        currentBackward->fBackward = currentBackward->gBackward + epsilon * hvalueBackward(currentBackward, env);
        currentBackward->parentBackward = NULL;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].visitedBackward = true;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].link = currentBackward;
        QList<POSITION *> openlistBackward;    // Backward priority Queue
        minHeapInsertB(&openlistBackward, currentBackward, env);
        int numberOfNodesExpandedBackward = 0;
        //currentBackward->fForward = MY_INFINITY;
        //currentBackward->gForward = MY_INFINITY;

        // Draw one time image
        QImage *image;
        if (!globalChkZoomIN && !globalChkZoomOUT)
            image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
        else
            image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid
        oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

        MYINT previousFvalueForward = 0;
        MYINT previousFvalueBackward =0;
        //while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward + 0.001f, openlistBackward.at(0)->fBackward + 0.001f) )
        while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward) )
        {
            //if (openlistForward.size() < openlistBackward.size())   // Expand Forward
            if ( numberOfNodesExpandedForward < numberOfNodesExpandedBackward )   // Expand Forward   // modifided to see how search grows
                //if (openlistForward.size() > openlistBackward.size())   // Expand Forward
            {
#ifdef  CAUTION
                if (!(current = extractMinF(&openlistForward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist forward returned NULL";
                    return 1;
                }
#else
                current = extractMinF(&openlistForward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes, as we have an epsilon bounded g_value
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

#ifdef MYASSERT
                if ( previousFvalueForward > current->fForward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueForward = current->fForward;
#endif
                numberOfNodesExpandedForward++;

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, true);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);
                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                            child->fForward = child->gForward = MY_INFINITY;
                        //if (child->gForward > current->gForward + cost(current, child))
                        if ( child->gForward > current->gForward + cost(current, child) )
                        {
                            child->gForward = current->gForward + cost(current, child);
                            child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                            minHeapInsertF(&openlistForward, child, env);
                            child->parentForward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;
                        }
                    }
                }
            }
            else        // Expand Backward
            {
#ifdef  CAUTION
                if (!(current = extractMinB(&openlistBackward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist backward returned NULL";
                    return 1;
                }
#else
                current = extractMinB(&openlistBackward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward = true;

#ifdef MYASSERT
                if ( previousFvalueBackward > current->fBackward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueBackward = current->fBackward;
#endif
                numberOfNodesExpandedBackward++;

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, false);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward)
                            child->fBackward = child->gBackward = MY_INFINITY;
                        //if (child->gBackward > current->gBackward + cost(current, child))
                        if ( child->gBackward > current->gBackward + cost(current, child) )
                        {
                            child->gBackward = current->gBackward + cost(current, child);
                            child->fBackward = child->gBackward + epsilon * hvalueBackward(child, env);
                            minHeapInsertB(&openlistBackward, child, env);
                            child->parentBackward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward = true;
                        }
                    }
                }
            }
        }   // end while

        // One end must have been reached
        env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
        env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
        env->epsilon = epsilon;

        qDebug() << "forw exp nodes" << numberOfNodesExpandedForward;
        qDebug() << "back exp nodes" << numberOfNodesExpandedBackward;
        qDebug() << "Total exp nodes" << numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
        qDebug() << "path cost " << costIncumbentSolution/1000;     // normalizing the cost
        qDebug() << "time elapsed " << QString::number(t.elapsed());
        if ( costIncumbentSolution < MY_INFINITY - 100 )
        {
#ifndef STOPIMAGEFILECREATION
            perPixelGridPainter(image, &(current->ptMe), true, false);
#endif
            return 0;   // successful
        }
        // Last paint call
        perPixelGridPainter(image, &(current->ptMe), true, false);

        // either of the goal is common
        /*      if(env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link->fForward == costIncumbentSolution)
        {
            printBidirAstarPath(env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link, env, &t, false);
            return 0;
        }
        else if(env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link->fBackward == costIncumbentSolution)
        {
            printBidirAstarPath(env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link, env, &t, false);
            return 0;
        }
        else    // none of the goal is common, so take max of the head of openlists
        {
            if (openlistForward.at(0)->fForward == costIncumbentSolution)
            {
                for (int i=0; i<openlistForward.size(); i++)
                {
                    if (openlistForward.at(i)->parentForward && openlistForward.at(i)->parentBackward)
                    {
                        printBidirAstarPath(openlistForward.at(0), env, &t, false);
                        return 0;
                    }
                }
            }
            else if (openlistBackward.at(0)->fBackward == costIncumbentSolution)
            {
                for (int i=0; i<openlistBackward.size(); i++)
                {
                    if (openlistBackward.at(i)->parentForward && openlistBackward.at(i)->parentBackward)
                    {
                        printBidirAstarPath(openlistBackward.at(0), env, &t, false);
                        return 0;   // success
                    }
                }
            }
        }
*/
        /*
        if(env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link &&
                env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link)       // both reached
        {
            printBidirAstarPath((env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link->fForward
                    < env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link->fBackward
                    ? env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link
                : env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link), env, &t, false);
            return 0;   // success
        }
        else if(env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link &&
                !env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link)      // forward only reached
        {
            printBidirAstarPath(env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link, env, &t, false);
            return 0;
        }
        else if(!env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link &&
                env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link)       // backward only reached
        {
            printBidirAstarPath(env->grid[env->ptGoalBackward.x()][env->ptGoalBackward.y()].link, env, &t, false);
            return 0;
        }
        */
    }
    else if (searchType == "sBWA*")
    {
        qDebug() << "==============================sBWA*=============================";   // basic sensing BWA*
        int incumbentSolutionCount = 0;
        QTime t;
        t.start();

        MYINT costIncumbentSolution = MY_INFINITY;

        int epsilon = 1;      // weight
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }
        env->epsilon = epsilon;         // for debugging heap

        POSITION *child = NULL, *current = NULL;

        POSITION *currentForward = new POSITION;    // Forward start node
        currentForward->ptMe = env->ptStartForward;
        currentForward->gForward = 0;
        currentForward->fForward = currentForward->gForward + epsilon * hvalueForward(currentForward, env);
        currentForward->parentForward = NULL;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].visitedForward = true;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].link = currentForward;
        QList<POSITION *> openlistForward;    // Forward priority Queue
        minHeapInsertF(&openlistForward, currentForward, env);
        int numberOfNodesExpandedForward = 0;
        //currentForward->fBackward = MY_INFINITY;
        //currentForward->gBackward = MY_INFINITY;

        POSITION *currentBackward = new POSITION;    // Backward start node
        currentBackward->ptMe = env->ptStartBackward;
        currentBackward->gBackward = 0;
        currentBackward->fBackward = currentBackward->gBackward + epsilon * hvalueBackward(currentBackward, env);
        currentBackward->parentBackward = NULL;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].visitedBackward = true;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].link = currentBackward;
        QList<POSITION *> openlistBackward;    // Backward priority Queue
        minHeapInsertB(&openlistBackward, currentBackward, env);
        int numberOfNodesExpandedBackward = 0;
        //currentBackward->fForward = MY_INFINITY;
        //currentBackward->gForward = MY_INFINITY;

        // Draw one time image
        QImage *image;
        if (!globalChkZoomIN && !globalChkZoomOUT)
            image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
        else
            image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid
        oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

        MYINT previousFvalueForward = 0;
        MYINT previousFvalueBackward =0;

        // ***********************************************SENSING***********************************************
#ifdef DEBUG
        QFile Anomalyfile("/home/peace/testResults/anomaly.txt");
        if (!Anomalyfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return 1;
        }
        QTextStream out1(&Anomalyfile);
#endif
#define SENSING_ARRAY_MAX_LIMIT 10
        int avgHvalueForward = MY_INFINITY, avgHvalueBackward = MY_INFINITY;
        int hValueArrayForward[SENSING_ARRAY_MAX_LIMIT] = {0}, hValueArrayBackward[SENSING_ARRAY_MAX_LIMIT] = {0};
        int indexCounterForward = 0, indexCounterBackward = 0;
        bool shortStuckForward = false, shortStuckBackward = false;
        int longStuckForward = 0, longStuckBackward = 0;
        int moveForward = true;
        // will measure newly expansions in stuck state, no less than 10% of the other side
        int freshExpansionForward = 1, freshExpansionBackward = 1;
        bool randModeForward = false, randModeBackward = false;
        // ***********************************************END SENSING***********************************************

        //while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward + 0.001f, openlistBackward.at(0)->fBackward + 0.001f) )
        while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward) )
        {
            // ****************************************SENSING****************************************
            if (  avgHvalueBackward<0 || avgHvalueForward<0 )
                qDebug() << "invalid exp";
            /* vesion 1
            if (!shortStuckForward)
                moveForward = true;
            else if (!shortStuckBackward)
                moveForward = false;
            else if (avgHvalueForward > avgHvalueBackward)
                moveForward = true;
            else
                moveForward = false;*/
            // version 2
            if (longStuckForward<longStuckBackward)
                moveForward = true;
            else
                moveForward = false;
            // ****************************************END SENSING****************************************
            //if (openlistForward.size() < openlistBackward.size())   // Expand Forward
            //if ( numberOfNodesExpandedForward < numberOfNodesExpandedBackward )   // Expand Forward
            if ( moveForward )   // Expand Forward
            {
#ifdef  CAUTION
                if (!(current = extractMinF(&openlistForward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist forward returned NULL";
                    return 1;
                }
#else
                current = extractMinF(&openlistForward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

#ifdef MYASSERT
                if ( previousFvalueForward > current->fForward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueForward = current->fForward;
#endif
                numberOfNodesExpandedForward++;

                // ****************************************SENSING*******************************************
                if (indexCounterForward==SENSING_ARRAY_MAX_LIMIT)
                {
                    double sumForward = 0;
                    for (int i=0; i<indexCounterForward; i++)
                        sumForward += hValueArrayForward[i];
                    int newAvg = sumForward/indexCounterForward;
                    longStuckForward += newAvg - avgHvalueForward;
                    /*if (newAvg > avgHvalueForward)  // new > previous; worsening condition
                        longStuckForward++;//shortStuckForward = true;
                    else
                        longStuckForward--;//shortStuckForward = false;*/
                    avgHvalueForward = newAvg;
                    /*double alpha = (double)(numberOfNodesExpandedForward-1-indexCounterForward)/(numberOfNodesExpandedForward-1);
                    avgHvalueForward = alpha*avgHvalueForward+sum/(numberOfNodesExpandedForward-1);*/
                    indexCounterForward = 0;
                    if (longStuckForward<0)
                        longStuckForward = 0;
#ifdef DEBUG
                    out1 << "-------forward--------" << endl;
                    out1 << "avghvalueforward " << (avgHvalueForward/1000) << "   longstuckforward " << (longStuckForward/1000) << "  ";
                    out1 << "x= " + QString::number(current->ptMe.x()) + "  y= " + QString::number(current->ptMe.y()) << "   ";
#endif
                }
                hValueArrayForward[indexCounterForward++] = hvalueForward(current, env);
                freshExpansionForward++;
                //if ((int)(0.1*freshExpansionForward) > freshExpansionBackward)
#ifdef DEBUG
            //out1 << "-------forward--------" << endl;
            //out1 << "f= " << current->fForward << "     g= " << current->gForward << "  h= " << current->fForward-current->gForward << endl;
            //out1 << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y()) << endl;
#endif
                // ******************************************END SENSING*******************************************

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, true);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);
                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                            child->fForward = child->gForward = MY_INFINITY;
                        //if (child->gForward > current->gForward + cost(current, child))
                        if ( child->gForward > current->gForward + cost(current, child) )
                        {
                            child->gForward = current->gForward + cost(current, child);
                            child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                            minHeapInsertF(&openlistForward, child, env);
                            child->parentForward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;
                        }
                    }
                }
            }
            else        // Expand Backward
            {
#ifdef  CAUTION
                if (!(current = extractMinB(&openlistBackward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist backward returned NULL";
                    return 1;
                }
#else
                current = extractMinB(&openlistBackward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward = true;

#ifdef MYASSERT
                if ( previousFvalueBackward > current->fBackward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueBackward = current->fBackward;
#endif
                numberOfNodesExpandedBackward++;

                // ***********************************************SENSING**************************************************
                if (indexCounterBackward==SENSING_ARRAY_MAX_LIMIT)
                {
                    double sumBackward = 0;
                    for (int i=0; i<indexCounterBackward; i++)
                        sumBackward += hValueArrayBackward[i];
                    int newAvg = sumBackward/indexCounterBackward;
                    longStuckBackward += newAvg - avgHvalueBackward;
                    /*if (newAvg > avgHvalueBackward)  // new > previous; worsening condition
                        longStuckBackward++;//shortStuckBackward = true;
                    else
                        longStuckBackward--;//shortStuckBackward = false;*/
                    avgHvalueBackward = newAvg;
                    /*double alpha = (double)(numberOfNodesExpandedBackward-1-indexCounterBackward)/(numberOfNodesExpandedBackward-1);
                    avgHvalueBackward = alpha*avgHvalueBackward+sum/(numberOfNodesExpandedBackward-1);*/
                    indexCounterBackward = 0;
                    if (longStuckBackward<0)
                        longStuckBackward = 0;
                }
                hValueArrayBackward[indexCounterBackward++] = hvalueBackward(current, env);
#ifdef DEBUG
            //out1 << "-------backward--------" << endl;
            //out1 << "f= " << current->fBackward << "     g= " << current->gBackward << "  h= " << current->fBackward-current->gBackward << endl;
            //out1 << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y()) << endl;
#endif
                // ***********************************************END SENSING***********************************************

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, false);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward)
                            child->fBackward = child->gBackward = MY_INFINITY;
                        //if (child->gBackward > current->gBackward + cost(current, child))
                        if ( child->gBackward > current->gBackward + cost(current, child) )
                        {
                            child->gBackward = current->gBackward + cost(current, child);
                            child->fBackward = child->gBackward + epsilon * hvalueBackward(child, env);
                            minHeapInsertB(&openlistBackward, child, env);
                            child->parentBackward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward = true;
                        }
                    }
                }
            }
        }   // end while
#ifdef DEBUG
        out1.flush();
        Anomalyfile.close();
#endif
        // One end must have been reached
        env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
        env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
        env->epsilon = epsilon;

        qDebug() << "forw exp nodes" << numberOfNodesExpandedForward;
        qDebug() << "back exp nodes" << numberOfNodesExpandedBackward;
        qDebug() << "Total exp nodes" << numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
        qDebug() << "path cost " << costIncumbentSolution/1000;     // normalizing the cost
        qDebug() << "time elapsed " << QString::number(t.elapsed());
        if ( costIncumbentSolution < MY_INFINITY - 100 )
        {
#ifndef STOPIMAGEFILECREATION
            perPixelGridPainter(image, &(current->ptMe), true, false);
#endif
            return 0;   // successful
        }
        // Last paint call
        perPixelGridPainter(image, &(current->ptMe), true, false);
    }
    else if (searchType == "rsBWA*")
    {
        qDebug() << "==============================rsBWA*=============================";  // random meta sensing BWA*
        //int incumbentSolutionCount = 0;
        QTime t;
        t.start();

        MYINT costIncumbentSolution = MY_INFINITY;

        int epsilon = 1;      // weight
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }
        env->epsilon = epsilon;         // for debugging heap

        POSITION *child = NULL, *current = NULL;

        POSITION *currentForward = new POSITION;    // Forward start node
        currentForward->ptMe = env->ptStartForward;
        currentForward->gForward = 0;
        currentForward->fForward = currentForward->gForward + epsilon * hvalueForward(currentForward, env);
        currentForward->parentForward = NULL;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].visitedForward = true;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].link = currentForward;
        QList<POSITION *> openlistForward;    // Forward priority Queue
        minHeapInsertF(&openlistForward, currentForward, env);
        int numberOfNodesExpandedForward = 0;
        //currentForward->fBackward = MY_INFINITY;
        //currentForward->gBackward = MY_INFINITY;

        POSITION *currentBackward = new POSITION;    // Backward start node
        currentBackward->ptMe = env->ptStartBackward;
        currentBackward->gBackward = 0;
        currentBackward->fBackward = currentBackward->gBackward + epsilon * hvalueBackward(currentBackward, env);
        currentBackward->parentBackward = NULL;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].visitedBackward = true;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].link = currentBackward;
        QList<POSITION *> openlistBackward;    // Backward priority Queue
        minHeapInsertB(&openlistBackward, currentBackward, env);
        int numberOfNodesExpandedBackward = 0;
        //currentBackward->fForward = MY_INFINITY;
        //currentBackward->gForward = MY_INFINITY;

        // Draw one time image
        QImage *image;
        if (!globalChkZoomIN && !globalChkZoomOUT)
            image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
        else
            image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid
        oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

        MYINT previousFvalueForward = 0;
        MYINT previousFvalueBackward =0;

        // ***********************************************SENSING***********************************************
#ifdef DEBUG
        QFile Anomalyfile("/home/peace/testResults/anomaly.txt");
        if (!Anomalyfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return 1;
        }
        QTextStream out1(&Anomalyfile);
#endif
#define SENSING_ARRAY_MAX_LIMIT 10
        int avgHvalueForward = MY_INFINITY, avgHvalueBackward = MY_INFINITY;
        int hValueArrayForward[SENSING_ARRAY_MAX_LIMIT] = {0}, hValueArrayBackward[SENSING_ARRAY_MAX_LIMIT] = {0};
        int indexCounterForward = 0, indexCounterBackward = 0;
        int longStuckForward = 0, longStuckBackward = 0;
        int moveForward = true;
        // will measure newly expansions in stuck state, no less than 10% of the other side
        int Rpercent = 10, Npercent = 5*10;
        int tempRpercent = 1;   // because one time execution without incremnt
        bool randModeForward = false, randModeBackward = false;
        // ***********************************************END SENSING***********************************************

        //while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward + 0.001f, openlistBackward.at(0)->fBackward + 0.001f) )
        while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward) )
        {
            // ****************************************SENSING****************************************
            if ( avgHvalueBackward<0 || avgHvalueForward<0 )
                qDebug() << "invalid exp";
            // META SEARCH
            if ( !randModeForward && !randModeBackward )
            {
                if ( Npercent <= qMax(numberOfNodesExpandedForward, numberOfNodesExpandedBackward) )
                {
                    Npercent *= 2;
                    if ( moveForward )
                    {
                        randModeBackward = true;
                        moveForward = false;
                    }
                    else
                    {
                        randModeForward = true;
                        moveForward = true;
                    }
                }
                else
                {
                    if ( longStuckForward < longStuckBackward )
                        moveForward = true;
                    else
                        moveForward = false;
                }
            }
            else
            {
                if ( randModeForward )
                {
                    if ( tempRpercent < Rpercent )
                    {
                        tempRpercent++;
                        moveForward = true;
                    }
                    else
                    {
                        Rpercent *= 2;
                        tempRpercent = 1;   // because one time execution without incremnt
                        randModeForward = false;
                    }
                }
                else    // randModeBackward == true
                {
                    if ( tempRpercent < Rpercent )
                    {
                        tempRpercent++;
                        moveForward = false;
                    }
                    else
                    {
                        Rpercent *= 2;
                        tempRpercent = 1;   // because one time execution without incremnt
                        randModeBackward = false;
                    }
                }
            }
            // ****************************************END SENSING****************************************
            //if (openlistForward.size() < openlistBackward.size())   // Expand Forward
            //if ( numberOfNodesExpandedForward < numberOfNodesExpandedBackward )   // Expand Forward
            if ( moveForward )   // Expand Forward
            {
#ifdef  CAUTION
                if (!(current = extractMinF(&openlistForward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist forward returned NULL";
                    return 1;
                }
#else
                current = extractMinF(&openlistForward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

#ifdef MYASSERT
                if ( previousFvalueForward > current->fForward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueForward = current->fForward;
#endif
                numberOfNodesExpandedForward++;

                // ****************************************SENSING*******************************************
                if (indexCounterForward==SENSING_ARRAY_MAX_LIMIT)
                {
                    double sumForward = 0;
                    for (int i=0; i<indexCounterForward; i++)
                        sumForward += hValueArrayForward[i];
                    int newAvg = sumForward/indexCounterForward;
                    longStuckForward += newAvg - avgHvalueForward;
                    avgHvalueForward = newAvg;
                    indexCounterForward = 0;
                    if (longStuckForward<0)
                        longStuckForward = 0;
#ifdef DEBUG
                    out1 << "-------forward--------" << endl;
                    out1 << "avghvalueforward " << (avgHvalueForward/1000) << "   longstuckforward " << (longStuckForward/1000) << "  ";
                    out1 << "x= " + QString::number(current->ptMe.x()) + "  y= " + QString::number(current->ptMe.y()) << "   ";
#endif
                }
                hValueArrayForward[indexCounterForward++] = hvalueForward(current, env);
#ifdef DEBUG
            //out1 << "-------forward--------" << endl;
            //out1 << "f= " << current->fForward << "     g= " << current->gForward << "  h= " << current->fForward-current->gForward << endl;
            //out1 << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y()) << endl;
#endif
                // ******************************************END SENSING*******************************************

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, true);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);
                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                            child->fForward = child->gForward = MY_INFINITY;
                        if ( child->gForward > current->gForward + cost(current, child) )
                        {
                            child->gForward = current->gForward + cost(current, child);
                            child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                            minHeapInsertF(&openlistForward, child, env);
                            child->parentForward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;
                        }
                    }
                }
            }
            else        // Expand Backward
            {
#ifdef  CAUTION
                if (!(current = extractMinB(&openlistBackward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist backward returned NULL";
                    return 1;
                }
#else
                current = extractMinB(&openlistBackward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward = true;

#ifdef MYASSERT
                if ( previousFvalueBackward > current->fBackward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueBackward = current->fBackward;
#endif
                numberOfNodesExpandedBackward++;

                // ***********************************************SENSING**************************************************
                if (indexCounterBackward==SENSING_ARRAY_MAX_LIMIT)
                {
                    double sumBackward = 0;
                    for (int i=0; i<indexCounterBackward; i++)
                        sumBackward += hValueArrayBackward[i];
                    int newAvg = sumBackward/indexCounterBackward;
                    longStuckBackward += newAvg - avgHvalueBackward;
                    avgHvalueBackward = newAvg;
                    indexCounterBackward = 0;
                    if (longStuckBackward<0)
                        longStuckBackward = 0;
                }
                hValueArrayBackward[indexCounterBackward++] = hvalueBackward(current, env);
#ifdef DEBUG
            //out1 << "-------backward--------" << endl;
            //out1 << "f= " << current->fBackward << "     g= " << current->gBackward << "  h= " << current->fBackward-current->gBackward << endl;
            //out1 << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y()) << endl;
#endif
                // ***********************************************END SENSING***********************************************

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, false);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward)
                            child->fBackward = child->gBackward = MY_INFINITY;
                        //if (child->gBackward > current->gBackward + cost(current, child))
                        if ( child->gBackward > current->gBackward + cost(current, child) )
                        {
                            child->gBackward = current->gBackward + cost(current, child);
                            child->fBackward = child->gBackward + epsilon * hvalueBackward(child, env);
                            minHeapInsertB(&openlistBackward, child, env);
                            child->parentBackward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward = true;
                        }
                    }
                }
            }
        }   // end while
#ifdef DEBUG
        out1.flush();
        Anomalyfile.close();
#endif
        // One end must have been reached
        env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
        env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
        env->epsilon = epsilon;

        qDebug() << "forw exp nodes" << numberOfNodesExpandedForward;
        qDebug() << "back exp nodes" << numberOfNodesExpandedBackward;
        qDebug() << "Total exp nodes" << numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
        qDebug() << "path cost " << costIncumbentSolution/1000;     // normalizing the cost
        qDebug() << "time elapsed " << QString::number(t.elapsed());
        if ( costIncumbentSolution < MY_INFINITY - 100 )
        {
#ifndef STOPIMAGEFILECREATION
            perPixelGridPainter(image, &(current->ptMe), true, false);
#endif
            return 0;   // successful
        }
        // Last paint call
        perPixelGridPainter(image, &(current->ptMe), true, false);
    }
    else if (searchType == "korfBWA*")
    {
        qDebug() << "==============================Barker & Korf - BWA*=============================";    // normal bi-WA*
        int incumbentSolutionCount = 0;
        QTime t;
        t.start();

        MYINT costIncumbentSolution = MY_INFINITY;

        int epsilon = 1;      // weight
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }
        env->epsilon = epsilon;         // for debugging heap

        POSITION *child = NULL, *current = NULL;

        POSITION *currentForward = new POSITION;    // Forward start node
        currentForward->ptMe = env->ptStartForward;
        currentForward->gForward = 0;
        currentForward->fForward = currentForward->gForward + epsilon * hvalueForward(currentForward, env);
        currentForward->parentForward = NULL;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].visitedForward = true;
        env->grid[currentForward->ptMe.x()][currentForward->ptMe.y()].link = currentForward;
        QList<POSITION *> openlistForward;    // Forward priority Queue
        minHeapInsertF(&openlistForward, currentForward, env);
        int numberOfNodesExpandedForward = 0;
        //currentForward->fBackward = MY_INFINITY;
        //currentForward->gBackward = MY_INFINITY;

        POSITION *currentBackward = new POSITION;    // Backward start node
        currentBackward->ptMe = env->ptStartBackward;
        currentBackward->gBackward = 0;
        currentBackward->fBackward = currentBackward->gBackward + epsilon * hvalueBackward(currentBackward, env);
        currentBackward->parentBackward = NULL;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].visitedBackward = true;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].link = currentBackward;
        QList<POSITION *> openlistBackward;    // Backward priority Queue
        minHeapInsertB(&openlistBackward, currentBackward, env);
        int numberOfNodesExpandedBackward = 0;
        //currentBackward->fForward = MY_INFINITY;
        //currentBackward->gForward = MY_INFINITY;

        // Draw one time image
        QImage *image;
        if (!globalChkZoomIN && !globalChkZoomOUT)
            image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
        else
            image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid
        oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

        MYINT previousFvalueForward = 0;
        MYINT previousFvalueBackward = 0;

        // for Barker & Korf
        int previousExpandedNodesForward = 0;
        int previousExpandedNodesBackward = 1;

        //while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward + 0.001f, openlistBackward.at(0)->fBackward + 0.001f) )
        while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward) )
        {
            //if (openlistForward.size() < openlistBackward.size())   // Expand Forward
            //if ( numberOfNodesExpandedForward < numberOfNodesExpandedBackward )   // Expand Forward   // modifided to see how search grows
            if ( previousExpandedNodesForward < previousExpandedNodesBackward )   // Expand Forward   // for Barker & Korf
                //if (openlistForward.size() > openlistBackward.size())   // Expand Forward
            {
#ifdef  CAUTION
                if (!(current = extractMinF(&openlistForward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist forward returned NULL";
                    return 1;
                }
#else
                current = extractMinF(&openlistForward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes, as we have an epsilon bounded g_value
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

#ifdef MYASSERT
                if ( previousFvalueForward > current->fForward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueForward = current->fForward;
#endif
                numberOfNodesExpandedForward++;

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, true);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);
                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                // for Barker & Korf
                previousExpandedNodesForward = 0;

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                            child->fForward = child->gForward = MY_INFINITY;
                        //if (child->gForward > current->gForward + cost(current, child))
                        if ( child->gForward > current->gForward + cost(current, child) )
                        {
                            child->gForward = current->gForward + cost(current, child);
                            child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                            minHeapInsertF(&openlistForward, child, env);
                            child->parentForward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;

                            // for Barker & Korf
                            previousExpandedNodesForward++;
                        }
                    }
                }
            }
            else        // Expand Backward
            {
#ifdef  CAUTION
                if (!(current = extractMinB(&openlistBackward, env)))   // take the first node from the OPEN
                {
                    qDebug() << "FAILURE: openlist backward returned NULL";
                    return 1;
                }
#else
                current = extractMinB(&openlistBackward, env);   // take the first node from the OPEN
#endif
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward = true;

#ifdef MYASSERT
                if ( previousFvalueBackward > current->fBackward && epsilon == 1 )
                {
                    qDebug() << "Assertion on non-decreasing fValue failed!";
                    qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                    qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                    return 1;
                }
                previousFvalueBackward = current->fBackward;
#endif
                numberOfNodesExpandedBackward++;

                // Paint the current pixel
                perPixelGridPainter(image, &(current->ptMe), false, false);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                     costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;

                    /*continue;
                    ++incumbentSolutionCount;
                    qDebug() << incumbentSolutionCount;*/
                }

                QVector<POSITION*> pVect;     // vector of children of current
                findChildren_(&pVect, current, env);

                // for Barker & Korf
                previousExpandedNodesBackward = 0;

                if (!pVect.size())
                    continue;
                else        // set ptParent for each child
                {
                    while(pVect.size())
                    {
                        child = pVect.takeLast();
                        if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward)
                            child->fBackward = child->gBackward = MY_INFINITY;
                        //if (child->gBackward > current->gBackward + cost(current, child))
                        if ( child->gBackward > current->gBackward + cost(current, child) )
                        {
                            child->gBackward = current->gBackward + cost(current, child);
                            child->fBackward = child->gBackward + epsilon * hvalueBackward(child, env);
                            minHeapInsertB(&openlistBackward, child, env);
                            child->parentBackward = current;
                            env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward = true;

                            // for Barker & Korf
                            previousExpandedNodesBackward++;
                        }
                    }
                }
            }
        }   // end while

        // One end must have been reached
        env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
        env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
        env->epsilon = epsilon;

        qDebug() << "forw exp nodes" << numberOfNodesExpandedForward;
        qDebug() << "back exp nodes" << numberOfNodesExpandedBackward;
        qDebug() << "Total exp nodes" << numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
        qDebug() << "path cost " << costIncumbentSolution/1000;     // normalizing the cost
        qDebug() << "time elapsed " << QString::number(t.elapsed());
        if ( costIncumbentSolution < MY_INFINITY - 100 )
        {
#ifndef STOPIMAGEFILECREATION
            perPixelGridPainter(image, &(current->ptMe), true, false);
#endif
            return 0;   // successful
        }
        // Last paint call
        perPixelGridPainter(image, &(current->ptMe), true, false);
    }
    return 1;
}

bool compareFloatValues(double f1, double f2)   // returns true if f1>f2 upto 3 decimal places
{
    int f1int;
    if (f1 > MY_INFINITY - 10000)
        f1int = MY_INFINITY;
    else
        f1int = f1*1000;
    int f2int = f2*1000;
    //qDebug() << "f1 = " << f1int << " f2 = " << f2int;
    if ( f1int > f2int )
        return true;
    else
        return false;
}

//@function     ARA* fvalue forward
MYINT fvalueForward(POSITION* current, Environment *env, int *epsilon)
{
    return current->gForward + (*epsilon) * hvalueForward(current, env);
}

//@function     ARA* fvalue backward
MYINT fvalueBackward(POSITION* current, Environment *env, int *epsilon)
{
    return current->gBackward + (*epsilon) * hvalueBackward(current, env);
}

//@function     second function of ARA*
int improvePath(POSITION *goal, int epsilon, QList<POSITION *> *openlist, QHash<QString, POSITION*> *inconslist, Environment *env)
{
    QTime t;
    t.start();

    int numberOfNodesExpandedForward = 0;
    QHash<QString, bool> closedlist;
    POSITION *current=NULL, *child=NULL;
    MYINT previousFvalue = 0;

    // Draw one time image
    QImage *image;
    if (!globalChkZoomIN && !globalChkZoomOUT)
        image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
    else
        image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid

    oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                       env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

    // START - heap debug [13-June-15]
    QList<POSITION *> openlistcopy;
    QList<POSITION *> decreaseKeyNodeChain;
    // END - heap debug [13-June-15]

#ifdef  CAUTION_NO
    while(openlist->at(0)->ptMe != goal->ptMe)
        //while(goal->f >= openlist->at(0)->f && openlist->at(0)->ptMe != goal->ptMe)
    {
        if (!(current = extractMinF(openlist, env)))   // take the first node from the OPEN
        {
            qDebug() << "FAILURE: openlist returned NULL";
            return 1;
        }
#else
    while(goal->fForward > openlist->at(0)->fForward)   // conflict
    {
        if ( !(current = extractMinF(openlist, env)) )   // take the first node from the OPEN
        {
            composeImageFile(image, true);
            qDebug() << "FAILURE in improvePATH(): openlist returned NULL";
            return 1;
        }
#endif

#ifdef MYASSERT
        if ( (previousFvalue > current->fForward) && epsilon == 1 )
        {
            qDebug() << "Assertion on non-decreasing fValue failed!";
            qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
            qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
            return 1;
        }
        previousFvalue = current->fForward;
#endif
        numberOfNodesExpandedForward++;     // no re-expansion condition checking as we are using closed list

        // Paint the current pixel
#ifndef STOPIMAGEFILECREATION
        perPixelGridPainter(image, &(current->ptMe), false, true);
#endif
        //expandlist.push_back(current->ptMe);  // temp
        if ( !closedlist.value(QString::number(current->ptMe.x())+"ab"+QString::number(current->ptMe.y()), false) )
            closedlist.insert(QString::number(current->ptMe.x())+"ab"+QString::number(current->ptMe.y()), true);
        else
        {
            qDebug() << QString::number(current->ptMe.x())+"ab"+QString::number(current->ptMe.y());
            qDebug() << "Error in closed list assignment in improvePath()";
            qDebug() << "current = " << current->ptMe;
            return 1;
        }

        // print current
        //qDebug() << "----------------------------";
        //qDebug() << closedlist.value(QString::number(current->ptMe.x())+"ab"+QString::number(current->ptMe.y()));
        //qDebug() << current->ptMe;

        QVector<POSITION*> childVect;     // vector of children of current
        findChildren_(&childVect, current, env);

        if (!childVect.size())
            continue;
        else        // set ptParent for each child
        {
            while(childVect.size())
            {
                child = childVect.takeLast();
                if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                    child->gForward = MY_INFINITY;
                if ( child->gForward > current->gForward + cost(current, child) )       // cost to any child is 1 or sqrt(2)
                {
                    child->gForward = current->gForward + cost(current, child);
                    child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                    child->parentForward = current;
                    env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;

//#ifdef MYASSERT
                    if ( child->fForward < current->fForward && epsilon == 1 )
                    {
                        qDebug() << "Great error, child-current f-value comparision... Terminating...";
                        return 1;
                    }
//#endif

                    if ( !closedlist.value(QString::number(child->ptMe.x())+"ab"+QString::number(child->ptMe.y()), false) )
                    //if ( !env->grid[child->ptMe.x()][child->ptMe.y()].onClosedListForward )
                    {
                        minHeapInsertF(openlist, child, env, &decreaseKeyNodeChain);
                    }
                    else    // insert into INCONS LIST
                    {
                        // check to avoid multiple insertions
                        if ( !inconslist->value(QString::number(child->ptMe.x())+"ab"+QString::number((child->ptMe.y())), NULL) )
                            inconslist->insert(QString::number(child->ptMe.x())+"ab"+QString::number((child->ptMe.y())), child);
                    }
#ifdef MYASSERT_VALIDATE_HEAP
                    if (epsilon==1 && numberOfNodesExpandedForward > 1000000)
                    {
                        validateHeap(openlist, current, &openlistcopy, &decreaseKeyNodeChain);     // heap debug [12-June-15]
                        qDebug() << numberOfNodesExpandedForward;   // heap debug [12-June-15]
                        decreaseKeyNodeChain.clear();
                        openlistcopy.clear();
                        for ( int i=0; i<openlist->size(); i++)
                            openlistcopy.append(openlist->at(i));
                    }
#endif
                }
            }
        }
    }
    //qDebug() << QString::number(*epsilon) +" bounded expanded nodes:  " + QString::number(numberOfNodesExpanded);
    env->arastarTotalNodesExpanded += numberOfNodesExpandedForward;
    qDebug() << "epsilon " << epsilon;
    qDebug() << "forw exp nodes " << numberOfNodesExpandedForward;
    qDebug() << "path cost " << goal->gForward/1000;    // normalizing the cost
    qDebug() << "time elapsed " << QString::number(t.elapsed());

#ifndef STOPIMAGEFILECREATION
    if (!current)
        perPixelGridPainter(image, &(env->ptGoalForward), true, false);
    else
        perPixelGridPainter(image, &(current->ptMe), true, false);
#endif

    /*#ifdef CAUTION
    if (openlist->at(0)->ptMe == env->ptGoalForward)    // goal expanded
    {
        //sortQPoint(&expandlist);  // temp
        env->epsilon = *epsilon;
        printARAstarPath(openlist->at(0), env, &t);
        return 0;   // success
    }
#else
    //sortQPoint(&expandlist);  // temp
    env->epsilon = *epsilon;
    printARAstarPath(env->grid[env->ptGoalForward.x()][env->ptGoalForward.y()].link, env, &t);
    return 0;   // success
#endif*/
    return 0;
}

//@function     second function of BARA*
int improvePathBidir(int epsilon, QList<POSITION *> *openlistForward, QHash<QString, POSITION*> *inconslistForward,
                     QList<POSITION *> *openlistBackward, QHash<QString, POSITION*> *inconslistBackward, Environment *env)
{
    QTime t;
    t.start();

    static MYINT costIncumbentSolution = MY_INFINITY;

    //QList<QPoint> expandlist;   // temp
    int numberOfNodesExpandedForward = 0;
    int numberOfNodesExpandedBackward = 0;
    QHash<QString, bool> closedlistForward;
    QHash<QString, bool> closedlistBackward;
    POSITION *current=NULL, *child=NULL;
    MYINT previousFvalueForward = 0;
    MYINT previousFvalueBackward = 0;

    // Draw one time image
    QImage *image;
    if (!globalChkZoomIN && !globalChkZoomOUT)
        image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
    else
        image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid

    oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                       env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

    /*while(goal->fForward > openlistForward->at(0)->fForward)   // conflict
    {
        if ( !(current = extractMinF(openlistForward, env)) )   // take the first node from the OPEN
        {
            composeImageFile(image, true);
            qDebug() << "FAILURE in improvePATH(): openlist returned NULL";
            return 1;
        }

#ifdef MYASSERT
            if ( (previousFvalueForward > current->fForward) && epsilon == 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueForward = current->fForward;
#endif
        numberOfNodesExpandedForward++;

        // Paint the current pixel
#ifndef STOPIMAGEFILECREATION
        perPixelGridPainter(image, &(current->ptMe), false, true);
#endif
        //expandlist.push_back(current->ptMe);  // temp
        if (!closedlistForward.value(QString::number(current->ptMe.x())+QString::number(current->ptMe.y())))
            closedlistForward.insert(QString::number(current->ptMe.x())+QString::number(current->ptMe.y()), true);

        // print current
        //qDebug() << "----------------------------";
        //qDebug() << closedlist.value(QString::number(current->ptMe.x())+QString::number(current->ptMe.y()));
        //qDebug() << current->ptMe;

        QVector<POSITION*> pVect;     // vector of children of current
        findChildren_(&pVect, current, env);

        if (!pVect.size())
            continue;
        else        // set ptParent for each child
        {
            while(pVect.size())
            {
                child = pVect.takeLast();
                if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                    child->gForward = MY_INFINITY;
                if ( child->gForward > current->gForward + cost(current, child) )       // cost to any child is 1
                {
                    child->gForward = current->gForward + cost(current, child);
                    child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                    child->parentForward = current;
                    env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;

                    if ( !closedlistForward.value(QString::number(child->ptMe.x())+QString::number(child->ptMe.y())) )
                        minHeapInsertF(openlistForward, child, env);//insertInList(*openlist, child);
                    else    // insert into INCONS LIST
                    {
                        if ( !inconslistForward->value(QString::number(child->ptMe.x())+QString::number((child->ptMe.y()))) )
                            inconslistForward->insert(QString::number(child->ptMe.x())+QString::number((child->ptMe.y())), child);
                    }
                }
            }
        }
    }
*/
    //-///////\\\\\\\\///////\\\\\\\\\\/////////-------/////\\\\\\\-------//////\\\\\\-------//////\\\\\\\------//////\\\\\\\//----------////\\\\\//-//
    while( costIncumbentSolution > qMax(openlistForward->at(0)->fForward, openlistBackward->at(0)->fBackward) )
    {
        if ( numberOfNodesExpandedForward < numberOfNodesExpandedBackward )   // Expand Forward
        {
#ifdef  CAUTION
            if (!(current = extractMinF(openlistForward, env)))   // take the first node from the OPEN
            {
                qDebug() << "FAILURE: openlist forward returned NULL";
                return 1;
            }
#else
            current = extractMinF(openlistForward, env);   // take the first node from the OPEN
#endif
            env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

#ifdef MYASSERT
            if ( previousFvalueForward > current->fForward && epsilon == 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueForward = current->fForward;
#endif
            numberOfNodesExpandedForward++;
            // Paint the current pixel
            perPixelGridPainter(image, &(current->ptMe), false, true);

            if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                 costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
            {
                costIncumbentSolution = current->gForward + current->gBackward;
                //qDebug() << "cost" << costIncumbentSolution;
                //qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                /*continue;
                ++incumbentSolutionCount;
                qDebug() << incumbentSolutionCount;*/
            }

            QVector<POSITION*> pVect;     // vector of children of current
            findChildren_(&pVect, current, env);

            if (!pVect.size())
                continue;
            else        // set ptParent for each child
            {
                while(pVect.size())
                {
                    child = pVect.takeLast();
                    if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                        child->fForward = child->gForward = MY_INFINITY;
                    if ( child->gForward > current->gForward + cost(current, child) )
                    {
                        child->gForward = current->gForward + cost(current, child);
                        child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                        child->parentForward = current;
                        env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;

                        if ( !env->grid[child->ptMe.x()][child->ptMe.y()].onClosedListForward )
                            minHeapInsertF(openlistForward, child, env);
                        else    // insert into INCONS LIST
                        {
                            if ( !inconslistForward->value(QString::number(child->ptMe.x())+QString::number((child->ptMe.y()))) )
                                inconslistForward->insert(QString::number(child->ptMe.x())+QString::number((child->ptMe.y())), child);
                        }
                    }
                }
            }
        }
        else        // Expand Backward
        {
#ifdef  CAUTION
            if (!(current = extractMinB(openlistBackward, env)))   // take the first node from the OPEN
            {
                qDebug() << "FAILURE: openlist backward returned NULL";
                return 1;
            }
#else
            current = extractMinB(openlistBackward, env);   // take the first node from the OPEN
#endif
            env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward = true;

#ifdef MYASSERT
            if ( previousFvalueBackward > current->fBackward && epsilon == 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueBackward = current->fBackward;
#endif
            numberOfNodesExpandedBackward++;
            // Paint the current pixel
            perPixelGridPainter(image, &(current->ptMe), false, false);

            if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                 costIncumbentSolution > current->gForward + current->gBackward )    // current already expanded by backward
            {
                costIncumbentSolution = current->gForward + current->gBackward;
                //qDebug() << "cost" << costIncumbentSolution;
                //qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                /*continue;
                ++incumbentSolutionCount;
                qDebug() << incumbentSolutionCount;*/
            }

            QVector<POSITION*> pVect;     // vector of children of current
            findChildren_(&pVect, current, env);

            if (!pVect.size())
                continue;
            else        // set ptParent for each child
            {
                while(pVect.size())
                {
                    child = pVect.takeLast();
                    if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward)
                        child->fBackward = child->gBackward = MY_INFINITY;
                    if ( child->gBackward > current->gBackward + cost(current, child) )
                    {
                        child->gBackward = current->gBackward + cost(current, child);
                        child->fBackward = child->gBackward + epsilon * hvalueBackward(child, env);
                        child->parentBackward = current;
                        env->grid[child->ptMe.x()][child->ptMe.y()].visitedBackward = true;

                        if ( !env->grid[child->ptMe.x()][child->ptMe.y()].onClosedListBackward )
                            minHeapInsertB(openlistBackward, child, env);
                        else    // insert into INCONS LIST
                        {
                            if ( !inconslistBackward->value(QString::number(child->ptMe.x())+QString::number((child->ptMe.y()))) )
                                inconslistBackward->insert(QString::number(child->ptMe.x())+QString::number((child->ptMe.y())), child);
                        }
                    }
                }
            }
        }
    }   // end while

    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
    env->arastarTotalNodesExpanded += numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
    env->epsilon = epsilon;

    qDebug() << "epsilon " << epsilon;
    qDebug() << "forw exp nodes " << numberOfNodesExpandedForward;
    qDebug() << "back exp nodes " << numberOfNodesExpandedBackward;
    qDebug() << "Total exp nodes" << numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
    qDebug() << "path cost " << costIncumbentSolution/1000;     // normalizing the cost
    qDebug() << "time elapsed " << QString::number(t.elapsed());

    if ( costIncumbentSolution < MY_INFINITY - 100 )
    {
#ifndef STOPIMAGEFILECREATION
        if (!current)
            perPixelGridPainter(image, &(env->ptGoalForward), true, false);
        else
            perPixelGridPainter(image, &(current->ptMe), true, false);
#endif
        return 0;   // successful
    }
    // Last paint call
    if (!current)
        perPixelGridPainter(image, &(env->ptGoalForward), true, false);
    else
        perPixelGridPainter(image, &(current->ptMe), true, false);

    return 1;
}

//@function     finds children of current node [uses link instead of visited]
void findChildren_(QVector<POSITION*> *pVect, POSITION* current, Environment* env)
{
    if (current->ptMe.x()-1 >= 0)   // left exists
    {
        if(env->grid[current->ptMe.x()-1][current->ptMe.y()].pathExists)
        {
            if (!env->grid[current->ptMe.x()-1][current->ptMe.y()].link)
            {
                POSITION *temp = new POSITION;
                temp->ptMe.setX(current->ptMe.x()-1);
                temp->ptMe.setY(current->ptMe.y());
                temp->parentForward = NULL;
                temp->parentBackward = NULL;
                pVect->push_back(temp);
                env->grid[current->ptMe.x()-1][current->ptMe.y()].link = temp;
            }
            else
                pVect->push_back(env->grid[current->ptMe.x()-1][current->ptMe.y()].link);
        }
        if(current->ptMe.y()-1 >= 0)     // top exists
        {
            if(env->grid[current->ptMe.x()-1][current->ptMe.y()-1].pathExists)
            {
                if(!env->grid[current->ptMe.x()-1][current->ptMe.y()-1].link)
                {
                    POSITION *temp = new POSITION;
                    temp->ptMe.setX(current->ptMe.x()-1);
                    temp->ptMe.setY(current->ptMe.y()-1);
                    temp->parentForward = NULL;
                    temp->parentBackward = NULL;
                    pVect->push_back(temp);
                    env->grid[current->ptMe.x()-1][current->ptMe.y()-1].link = temp;
                }
                else
                    pVect->push_back(env->grid[current->ptMe.x()-1][current->ptMe.y()-1].link);
            }
        }
        if (current->ptMe.y()+1 < env->ptGridSize.y())  // bottom exists
        {
            if (env->grid[current->ptMe.x()-1][current->ptMe.y()+1].pathExists)
            {
                if(!env->grid[current->ptMe.x()-1][current->ptMe.y()+1].link)
                {
                    POSITION *temp = new POSITION;
                    temp->ptMe.setX(current->ptMe.x()-1);
                    temp->ptMe.setY(current->ptMe.y()+1);
                    temp->parentForward = NULL;
                    temp->parentBackward = NULL;
                    pVect->push_back(temp);
                    env->grid[current->ptMe.x()-1][current->ptMe.y()+1].link = temp;
                }
                else
                    pVect->push_back(env->grid[current->ptMe.x()-1][current->ptMe.y()+1].link);
            }
        }
    }
    if (current->ptMe.x()+1 < env->ptGridSize.x())  // right exists
    {
        if(env->grid[current->ptMe.x()+1][current->ptMe.y()].pathExists)
        {
            if(!env->grid[current->ptMe.x()+1][current->ptMe.y()].link)
            {
                POSITION *temp = new POSITION;
                temp->ptMe.setX(current->ptMe.x()+1);
                temp->ptMe.setY(current->ptMe.y());
                temp->parentForward = NULL;
                temp->parentBackward = NULL;
                pVect->push_back(temp);
                env->grid[current->ptMe.x()+1][current->ptMe.y()].link = temp;
            }
            else
                pVect->push_back(env->grid[current->ptMe.x()+1][current->ptMe.y()].link);
        }
        if(current->ptMe.y()-1 >= 0)     // top exists
        {
            if(env->grid[current->ptMe.x()+1][current->ptMe.y()-1].pathExists)
            {
                if(!env->grid[current->ptMe.x()+1][current->ptMe.y()-1].link)
                {
                    POSITION *temp = new POSITION;
                    temp->ptMe.setX(current->ptMe.x()+1);
                    temp->ptMe.setY(current->ptMe.y()-1);
                    temp->parentForward = NULL;
                    temp->parentBackward = NULL;
                    pVect->push_back(temp);
                    env->grid[current->ptMe.x()+1][current->ptMe.y()-1].link = temp;
                }
                else
                    pVect->push_back(env->grid[current->ptMe.x()+1][current->ptMe.y()-1].link);
            }
        }
        if (current->ptMe.y()+1 < env->ptGridSize.y())  // bottom exists
        {
            if (env->grid[current->ptMe.x()+1][current->ptMe.y()+1].pathExists)
            {
                if(!env->grid[current->ptMe.x()+1][current->ptMe.y()+1].link)
                {
                    POSITION *temp = new POSITION;
                    temp->ptMe.setX(current->ptMe.x()+1);
                    temp->ptMe.setY(current->ptMe.y()+1);
                    temp->parentForward = NULL;
                    temp->parentBackward = NULL;
                    pVect->push_back(temp);
                    env->grid[current->ptMe.x()+1][current->ptMe.y()+1].link = temp;
                    //qDebug() << "heapIndex " + QString::number(env->grid[current->ptMe.x()+1][current->ptMe.y()+1].heapIndex);
                }
                else
                    pVect->push_back(env->grid[current->ptMe.x()+1][current->ptMe.y()+1].link);
            }
        }
    }
    if (current->ptMe.y()-1 >= 0)   // top exists
    {
        if (env->grid[current->ptMe.x()][current->ptMe.y()-1].pathExists)
        {
            if(!env->grid[current->ptMe.x()][current->ptMe.y()-1].link)
            {
                POSITION *temp = new POSITION;
                temp->ptMe.setX(current->ptMe.x());
                temp->ptMe.setY(current->ptMe.y()-1);
                temp->parentForward = NULL;
                temp->parentBackward = NULL;
                pVect->push_back(temp);
                env->grid[current->ptMe.x()][current->ptMe.y()-1].link = temp;
            }
            else
                pVect->push_back(env->grid[current->ptMe.x()][current->ptMe.y()-1].link);
        }
    }
    if (current->ptMe.y()+1 < env->ptGridSize.y())   // bottom exists
    {
        if (env->grid[current->ptMe.x()][current->ptMe.y()+1].pathExists)
        {
            if(!env->grid[current->ptMe.x()][current->ptMe.y()+1].link)
            {
                POSITION *temp = new POSITION;
                temp->ptMe.setX(current->ptMe.x());
                temp->ptMe.setY(current->ptMe.y()+1);
                temp->parentForward = NULL;
                temp->parentBackward = NULL;
                pVect->push_back(temp);
                env->grid[current->ptMe.x()][current->ptMe.y()+1].link = temp;
            }
            else
                pVect->push_back(env->grid[current->ptMe.x()][current->ptMe.y()+1].link);
        }
    }
}

//@function     find forward heuristic value: straight line distance
MYINT hvalueForward(POSITION * current, Environment *env)
{
    int diffx = abs(env->ptGoalForward.x()-current->ptMe.x());
    int diffy = abs(env->ptGoalForward.y()-current->ptMe.y());
    return (diffx > diffy)?(diffy*DCOST+(diffx-diffy)*HCOST):(diffx*DCOST+(diffy-diffx)*HCOST);
}

//@function     find backward heuristic value: straight line distance
MYINT hvalueBackward(POSITION * current, Environment *env)
{
    int diffx = abs(env->ptGoalBackward.x()-current->ptMe.x());
    int diffy = abs(env->ptGoalBackward.y()-current->ptMe.y());
    return (diffx > diffy)?(diffy*DCOST+(diffx-diffy)*HCOST):(diffx*DCOST+(diffy-diffx)*HCOST);
}

//@function     find cost between two nodes
MYINT cost(POSITION * node1, POSITION *node2)
{
    int diffx = abs(node2->ptMe.x()-node1->ptMe.x());
    int diffy = abs(node2->ptMe.y()-node1->ptMe.y());
    return (diffx > diffy)?(diffy*DCOST+(diffx-diffy)*HCOST):(diffx*DCOST+(diffy-diffx)*HCOST);
}

//@function     inserts node as per priority
void insertInList(QList<POSITION*> &openlist, POSITION* current)
{
    POSITION* temp;
    int x = 0;
    QList<POSITION*>::iterator itr;
    itr = openlist.begin();
    while(itr!=openlist.end())
    {
        temp = (*itr);
        if (current == temp)
        {
            openlist.removeAt(x);
            break;
        }
        itr++;
        x++;
    }
    itr = openlist.begin();
    while (itr!=openlist.end())
    {
        temp = (*itr);
        if(current->fForward < temp->fForward)
        {
            openlist.insert(itr,current);
            return;
        }
        itr++;
    }
    openlist.insert(itr,current);
}

//@function     return a dynamically allocated 2D array
template <typename T>
T **AllocateDynamicArray(int nRows, int nCols)
{
    T **dynamicArray;

    dynamicArray = new T*[nRows];
    for(int i = 0; i < nRows; i++)
        dynamicArray[i] = new T [nCols];

    return dynamicArray;
}

//@function     delete the dynamic memory allocated to a 2D array
template <typename T>
void FreeDynamicArray(T** dArray)
{
    delete [] *dArray;
    delete [] dArray;
}

//@function     clear the previous groupBox elements
void MainWindow::on_btnClearDrawing_clicked()
{

}

//@fuction      Set of all assert conditions
int assertCondition(int condType)
{
    /*
     * 1.   min-key should be non decreasing
     * 2.   while discovering children
     *          child->f >= parent->f                               //consistent heuristic
     *          child->f >= parent->h - cost(parent, child)         //consistent heuristic condition
     * 3.   if (s) belongs to optimal_path_B && was not expanded fwd
     *          then    parentF(s) was expanded(? F/B)
     */
}

//@function     Tells whether search has found a path or not
bool checkPathCorrectness(Environment *env, POSITION *startState, POSITION *goalState, bool forwardSearch)
{
    POSITION *current;
    current = goalState;
    QList<POSITION *> visitedNodeList;    // priority Queue
    visitedNodeList.append(current);
    while( current != startState )
    {
        if ( forwardSearch )
            current = current->parentForward;
        else
            current = current->parentBackward;

        if ( visitedNodeList.contains(current) || !current )    // i.e. current has already been visited before once
            return false;

        visitedNodeList.append(current);
    }
    return true;
}

//@function     Zoom IN to the image
QImage composeZoomedINimageFile(QImage *image)
{
    int zoomTimes = globalZoomTimes;
    if (zoomTimes<1) zoomTimes = 1;
    QImage tempimage = image->scaled(image->width()*zoomTimes, image->height()*zoomTimes, Qt::KeepAspectRatio, Qt::FastTransformation);
    return tempimage;
    // MY CODE
    /*int zoomTimes = globalZoomTimes;
    QImage tempimage(image->width()*zoomTimes, image->height()*zoomTimes, QImage::Format_RGB32);

    {
        for (int j=0; j < image->height(); j++)
        {
            for (int otimes=0; otimes<zoomTimes; otimes++)
            {
                for (int i=0; i < image->width(); i++)
                {
                    for (int times=0; times<zoomTimes; times++)
                    {
                        tempimage.setPixel(i*zoomTimes+times, j*zoomTimes + otimes, image->pixel(i,j));
                    }
                }
            }
        }
    }
    return tempimage;*/
}

//@function     Zoom OUT to the image
QImage composeZoomedOUTimageFile(QImage *image)
{
    int zoomOutTimes = globalZoomTimes;
    if (zoomOutTimes<1) zoomOutTimes = 1;
    QImage tempimage = image->scaled(image->width()/zoomOutTimes, image->height()/zoomOutTimes, Qt::KeepAspectRatio, Qt::FastTransformation);
    /*QImage tempimage(image->width()*zoomTimes, image->height()*zoomTimes, QImage::Format_RGB32);

    {
        for (int j=0; j < image->height(); j++)
        {
            for (int otimes=0; otimes<zoomTimes; otimes++)
            {
                for (int i=0; i < image->width(); i++)
                {
                    for (int times=0; times<zoomTimes; times++)
                    {
                        tempimage.setPixel(i*zoomTimes+times, j*zoomTimes + otimes, image->pixel(i,j));
                    }
                }
            }
        }
    }*/
    return tempimage;
}




















