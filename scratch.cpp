
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "structures.h"
#include "functions.h"
#include "fileformatterutility.h"
#include "blockcreator.h"

#include <QDebug>
#include <QVector>
#include <QPoint>
#include <QLabel>
#include <QImage>
#include <QRgb>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QMutableListIterator>
#include <QtMath>
#include <QRect>
#include <QTime>
#include <QLinkedList>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QApplication>
#include <QPainter>
#include <QScrollArea>
#include <QImageWriter>

QRgb globalColorValue = qRgb(255, 0, 0);    // Red in the beginning
int MY_INFINITY = 1000000000;   // 1 billion

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    createOutdoorBlockedConfigurationsCCL();
    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection); // very important line for terminating the entire application
    //QTextStream(stdout) << ">>>" + QCoreApplication::arguments().at(1) + " " + QCoreApplication::arguments().at(2) << endl;
}

void MainWindow::on_btnTestor_clicked()
{
    short **grid = AllocateDynamicArray<short>(4,3);
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
    float f1 = 0.7563f, f2 = 0.756f;
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
        qDebug() << "Error file opening the config file, Exiting...";
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
short prepareEnvironment(QString configFile, Environment& env)
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
    for(register int j=0; j<env.ptGridSize.y(); j++)   // increment y
    {
        line = in.readLine();
        if (line.isEmpty())
        {
            qDebug() << "error in configuration file.";
            break;
        }

        spaceSplitted = line.split(respace);
        register int k = 0;
        for(register int i=0;k<env.ptGridSize.x();i++)
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
}

void MainWindow::on_btnSearch_clicked()
{
    // search global variable
    Environment env;
    QString configFile = ui->lePath->text();  // read env file
    // initialize data structures: STACK, HEAP as per the searchType requirement
    // plot the environment and globals
    prepareEnvironment(configFile, env);

    // determine search type
    QString searchType = DEFAULT_SEARCH_TYPE;
    if (ui->leSearchType->text()=="astar")
        searchType = "astar";
    else if (ui->leSearchType->text()=="arastar")
        searchType = "arastar";
    else if (ui->leSearchType->text()=="bidirastar")
        searchType = "bidirastar";

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
        //MainWindow::guiHandler(&env);
        float epsilon = 1;
        if (searchType != "arastar" && searchType != "bidirarastar" && searchType != "bidirastar")
        {
#ifndef LARGESIZE
            newGuiHandler(&env, &env.epsilon);
#else
            ;
#endif
        }
    }
    else
        qDebug() << "Search was unsuccessful!! :(  Exiting...";

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

    qDebug() << "eps: " + QString::number(env->epsilon) + " Path cost     " + QString::number(pathlength);
    qDebug() << "eps: " + QString::number(env->epsilon) + " Expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward);
    qDebug() << "eps: " + QString::number(env->epsilon) + " Elapsed time = " + timeElapsed;
    qDebug() << "Total expanded nodes:  " + QString::number(env->arastarTotalNodesExpanded);
}

void printBidirAstarPath(POSITION *current, Environment* env, QTime *t, bool common)     // current is the meeting point
{
    static int printSequence;
    printSequence++;
    QString timeElapsed = QString::number(t->elapsed());
    float epsilon = env->epsilon;

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
    qDebug() << "Path cost =   " + QString::number(pathlength);
    qDebug() << "Forward expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward);
    qDebug() << "Backward expanded nodes:  " + QString::number(env->numberOfNodesExpandedBackward);
    qDebug() << "Total expanded nodes:  " + QString::number(env->numberOfNodesExpandedForward + env->numberOfNodesExpandedBackward);
    qDebug() << "Elapsed time =   " + timeElapsed;

#ifndef LARGESIZE
    bidirGuiHandler(env, &epsilon, &printSequence);
#endif

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
void composeImageFile(QImage *image, bool lastRound, QString _imageFilePath = "", float epsilon = 1)
{
    static int maxHit = 80000 + 2;  // set it true for first call
    static int fileNumber = 1;

    QString path;
    if ( _imageFilePath == "")
        path = "/home/peace/testResults/";
    else
        path = _imageFilePath;

    QString imageFilePath = path + "trace" + QString::number(fileNumber) + ".png";

    if ( maxHit> (epsilon * 80000) || lastRound)
    //if ( maxHit> (-1) || lastRound)
    {
        QImageWriter writer( imageFilePath, "png" );
        writer.setText("Author", "HB");
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
void perPixelGridPainter(QWidget *myWindow, QGridLayout *myGridLayout, QLabel *myLabel, QImage *image, QPoint *ptMe, bool lastRound, bool direction = false, QString imageFilePath = "", float epsilon = 1)
{
    QRgb value;
    if (direction)   // forward
    //value = qRgb(255, 0, 0); // Red
        value = globalColorValue;
    else        // backward
        value = qRgb(0, 0, 255);
    image->setPixel(*ptMe, value);

#ifndef WRITEIMAGE
    QPixmap inpixmap;
    inpixmap.fromImage(*image);
    //QPixmap output(inPixmap.width(), inPixmap.height());
    QPainter painter(&inpixmap);
    myLabel->render(&painter);

    myLabel->repaint();

    //myLabel->setPixmap(NULL);
    //myLabel->setPixmap( QPixmap::fromImage(*image) );
    QApplication::processEvents();
    //myLabel->setPicture();

    //myGridlayout->addWidget( myLabel, 0, 0 );

    //myWindow->setFixedHeight(IMAGE_Y);
    //myWindow->setFixedWidth(IMAGE_X);

    //myWindow->setLayout(myGridlayout);
    //if (!silent)    //  Display
    //myWindow->repaint();
    //myWindow->show();
#else
    composeImageFile(image, lastRound, imageFilePath, epsilon);
#endif
}

//@function     create QImage with intial config file
void oneTimeGridPainter(QWidget *myWindow, QGridLayout *myGridLayout, QLabel *myLabel, QImage *image, GRIDSTATE **displayGrid,
                        int IMAGE_X, int IMAGE_Y, int startX, int startY, int goalX, int goalY, QString imageFilePath = "")
{
#ifndef WRITEIMAGE
    myWindow = new QWidget;
    myWindow->setWindowTitle("Compressed Configuration");
    myGridLayout = new QGridLayout;
    myLabel = new QLabel("A");

    myGridLayout->setHorizontalSpacing(0);
    myGridLayout->setVerticalSpacing(0);
#endif
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

    /*QImageWriter writer( path + "image" + QString::number(fileCount) + ".png", "png" );
    writer.setText("Author", "HB");
    writer.write(image);*/

#ifndef WRITEIMAGE
    myLabel->setPixmap( QPixmap::fromImage(*image) );
    myGridLayout->addWidget( myLabel, 0, 0 );

    //myWindow->setFixedHeight(IMAGE_Y);
    //myWindow->setFixedWidth(IMAGE_X);
    myWindow->setMaximumHeight(IMAGE_Y/2);
    myWindow->setMinimumHeight(IMAGE_Y/3);
    myWindow->setMaximumWidth(IMAGE_X/2);
    myWindow->setMinimumWidth(IMAGE_X/3);
    myWindow->setWindowState(Qt::WindowMaximized);

    QScrollArea* scroller = new QScrollArea;
    scroller->setWidget(myLabel);

    qApp->processEvents();

    myWindow = scroller;
    //myWindow->setLayout(myGridLayout);
    myWindow->show();    //  Display
    qApp->processEvents();
    QApplication::processEvents();
#else
    composeImageFile(image, false, imageFilePath);
#endif
}

//@function     search
short MainWindow::visualSearch(QString searchType, Environment *env)
{
    //QList<QPoint> expandlist;   // temp
    if (searchType == "astar")
    {
        QTime t;
        t.start();

        float epsilon = 1;      // weight
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }

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
        int numberOfNodesExpanded = 0;

        // Draw one time image
        QWidget *myWindow;
        QGridLayout *myGridLayout;
        QLabel *myLabel;
        QImage image(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);
        oneTimeGridPainter(myWindow, myGridLayout, myLabel, &image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());
        //thread()->msleep(5000);

        int previousFvalue = 0;
        QFile Anomalyfile("/home/peace/testResults/anomaly.txt");
        if (!Anomalyfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return 1;
        }
        QTextStream out1(&Anomalyfile);

        while(current->ptMe != env->ptGoalForward)  // while(goal node is not expanded)
        {
#ifdef  CAUTION
            if (!(current = extractMinF(&openlist, env)))   // take the first node from the OPEN
            {
                composeImageFile(&image, true);
                qDebug() << "FAILURE: openlist returned NULL";
                return 1;
            }
#else
            current = extractMinF(&openlist, env);   // take the first node from the OPEN
#endif

#ifdef MYASSERT
            if ( compareFloatValues(previousFvalue, current->fForward) && epsilon - 1 < 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
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
#ifdef  DEBUG
            out1 << "---------------" << endl;
            out1 << "f= " << current->fForward << "     g= " << current->gForward << "  h= " << current->fForward-current->gForward << endl;
            out1 << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y()) << endl;
#endif

            //qDebug() << "point " << current->ptMe << " fvalue " << current->fForward;

            // Paint the current pixel
#ifndef STOPIMAGEFILECREATION
            perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), false, true);
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
                perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), true, false);
#endif
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
                    //if (child->gForward > current->gForward + (int)cost(current, child))
                    if ( compareFloatValues(child->gForward, current->gForward + cost(current, child)) )
                    {
                        child->gForward = current->gForward + cost(current, child);
                        child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                        minHeapInsertF(&openlist, child, env);
                        child->parentForward = current;
                        env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;
                    }
                }
            }
            //this->guiHandler(env);
            //QThread::sleep(1);
        }
        out1.flush();
        Anomalyfile.close();
        return 1;   // failure
    }
    else if (searchType == "arastar")
    {
        QTime t;
        t.start();

        float costIncumbentSolution = MY_INFINITY;

        float epsilon = 1;      // weight
        float decrement = 1;
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }


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
        QWidget *myWindow;
        QGridLayout *myGridLayout;
        QLabel *myLabel;
        QImage image(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);
        oneTimeGridPainter(myWindow, myGridLayout, myLabel, &image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

        int previousFvalueForward = 0;
        int previousFvalueBackward =0;
        //while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward + 0.001f, openlistBackward.at(0)->fBackward + 0.001f) )
        while( compareFloatValues(costIncumbentSolution, qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward)) )
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
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

#ifdef MYASSERT
            if ( compareFloatValues(previousFvalueForward, current->fForward) && epsilon - 1 < 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueForward = current->fForward;
#endif
                numberOfNodesExpandedForward++;
                env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;
                // Paint the current pixel
                perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), false, true);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                        compareFloatValues(costIncumbentSolution, current->gForward + current->gBackward) )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);
                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;
                    /*if (current->ptMe == env->ptGoalForward)
                        printBidirAstarPath(current, env, &t, false);
                    else
                        printBidirAstarPath(current, env, &t, true);*/

                    t.restart();    // reset the timer
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
                        if ( compareFloatValues(child->gForward, current->gForward + cost(current, child)) )
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
            if ( compareFloatValues(previousFvalueBackward, current->fBackward) && epsilon - 1 < 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueBackward = current->fBackward;
#endif
                numberOfNodesExpandedBackward++;
                env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward = true;
                // Paint the current pixel
                perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), false, false);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                        compareFloatValues(costIncumbentSolution, current->gForward + current->gBackward) )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;
                    //if (current->ptMe == env->ptGoalBackward)
                        //printBidirAstarPath(current, env, &t, false);
                    //else
                        //printBidirAstarPath(current, env, &t, true);

                    t.restart();    // reset the timer
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
                        if ( compareFloatValues(child->gBackward, current->gBackward + cost(current, child)) )
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
        qDebug() << "path cost" << costIncumbentSolution;
        if ( costIncumbentSolution < MY_INFINITY - 100 )
        {
#ifndef STOPIMAGEFILECREATION
                perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), true, false);
#endif
            return 0;   // successful
        }
        // Last paint call
        perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), true, false);

















        POSITION *current = new POSITION;    // start node
        current->ptMe = env->ptStartForward;

        QList<POSITION *> openlist;    // priority Queue
        QHash<QString, POSITION*> inconslist;

        //env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g = MY_INFINITY;
        //env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].f = fvalue(&env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()], &env, &epsilon);
        POSITION *goal = new POSITION;
        goal->ptMe = env->ptGoalForward;
        goal->parentForward = NULL;
        goal->gForward = MY_INFINITY;
        goal->fForward = fvalueForward(goal, env, &epsilon);
        env->grid[goal->ptMe.x()][goal->ptMe.y()].link = goal;

        current->gForward = 0;
        //current->visited = true;
        env->grid[current->ptMe.x()][current->ptMe.y()].visitedForward = true;
        env->grid[current->ptMe.x()][current->ptMe.y()].link = current;
        current->fForward = fvalueForward(current, env, &epsilon);
        //insertInList(openlist, current);
        current->parentForward = NULL;
        minHeapInsertF(&openlist, current, env);

        //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
#ifdef CAUTION
        if (improvePath(goal, &epsilon, &openlist, &inconslist, env))
        {
            qDebug() << "FAILURE: returning from improvePath";
            return 1;
        }
#else
        improvePath(goal, &epsilon, &openlist, &inconslist, env);
#endif

        while(epsilon > 1)  // while(goal node is not expanded)
        {
            epsilon = epsilon - decrement;

            // merge openlist with inconslist
            QHash<QString, POSITION*>::iterator inconsItr;
            for (inconsItr = inconslist.begin(); inconsItr != inconslist.end(); inconsItr++)
            {
                //inconslist.at(i)->f = fvalue(inconslist.at(i), env, &epsilon);
                if (inconsItr.value())
                    openlist.append(inconsItr.value());
            }
            // update f values for new epsilon
            for (register int i =0; i<openlist.size(); i++)
                openlist.at(i)->fForward = fvalueForward(openlist.at(i), env, &epsilon);

            /*for (register int i =0; i<openlist.size(); i++)     // sort
                for (register int j=i+1; j<openlist.size(); j++)
                    if (openlist.at(i)->f > openlist.at(j)->f)
                        openlist.swap(i,j);
            for (register int i =0; i<inconslist.size(); i++)     // sort
                for (register int j=i+1; j<inconslist.size(); j++)
                    if (inconslist.at(i)->f > inconslist.at(j)->f)
                        inconslist.swap(i,j);

            for (register int i=0,j=0; i<inconslist.size();)    // merge
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

            /*for(int i = 0; i<inconslist.size(); i++)
                minHeapInsert(&openlist, inconslist.takeFirst(), env);*/

            inconslist.clear();

#ifdef CAUTION
        if (improvePath(goal, &epsilon, &openlist, &inconslist, env))
        {
            qDebug() << "FAILURE: returning from improvePath";
            return 1;
        }
#else
        improvePath(goal, &epsilon, &openlist, &inconslist, env);
#endif

            //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
        }
        return 0;   // success
    }
    else if (searchType == "bidirastar")
    {
        QTime t;
        t.start();

        float costIncumbentSolution = MY_INFINITY;

        float epsilon = 1;      // weight
        if (!ui->leEpsilon->text().isEmpty())
        {
            bool ok;
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        }


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
        QWidget *myWindow;
        QGridLayout *myGridLayout;
        QLabel *myLabel;
        QImage image(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);
        oneTimeGridPainter(myWindow, myGridLayout, myLabel, &image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());

        int previousFvalueForward = 0;
        int previousFvalueBackward =0;
        //while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward + 0.001f, openlistBackward.at(0)->fBackward + 0.001f) )
        while( compareFloatValues(costIncumbentSolution, qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward)) )
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
                // Stopping reexpansion of nodes
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                    continue;
                else
                    env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

#ifdef MYASSERT
            if ( compareFloatValues(previousFvalueForward, current->fForward) && epsilon - 1 < 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedForward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueForward = current->fForward;
#endif
                numberOfNodesExpandedForward++;
                env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;
                // Paint the current pixel
                perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), false, true);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                        compareFloatValues(costIncumbentSolution, current->gForward + current->gBackward) )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);
                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;
                    /*if (current->ptMe == env->ptGoalForward)
                        printBidirAstarPath(current, env, &t, false);
                    else
                        printBidirAstarPath(current, env, &t, true);*/

                    t.restart();    // reset the timer
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
                        if ( compareFloatValues(child->gForward, current->gForward + cost(current, child)) )
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
            if ( compareFloatValues(previousFvalueBackward, current->fBackward) && epsilon - 1 < 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueBackward = current->fBackward;
#endif
                numberOfNodesExpandedBackward++;
                env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward = true;
                // Paint the current pixel
                perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), false, false);

                if ( env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                        compareFloatValues(costIncumbentSolution, current->gForward + current->gBackward) )    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
                    qDebug() << "cost" << costIncumbentSolution;
                    qDebug() << "qmax" << qMax(openlistForward.at(0)->fForward, openlistBackward.at(0)->fBackward);

                    env->numberOfNodesExpandedForward = numberOfNodesExpandedForward;
                    env->numberOfNodesExpandedBackward = numberOfNodesExpandedBackward;
                    env->epsilon = epsilon;
                    //if (current->ptMe == env->ptGoalBackward)
                        //printBidirAstarPath(current, env, &t, false);
                    //else
                        //printBidirAstarPath(current, env, &t, true);

                    t.restart();    // reset the timer
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
                        if ( compareFloatValues(child->gBackward, current->gBackward + cost(current, child)) )
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
        qDebug() << "path cost" << costIncumbentSolution;
        if ( costIncumbentSolution < MY_INFINITY - 100 )
        {
#ifndef STOPIMAGEFILECREATION
                perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), true, false);
#endif
            return 0;   // successful
        }
        // Last paint call
        perPixelGridPainter(myWindow, myGridLayout, myLabel, &image, &(current->ptMe), true, false);

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
    if ( f1int > f2int )
        return true;
    else
        return false;
}

//@function     arastar fvalue forward
float fvalueForward(POSITION* current, Environment *env, float *epsilon)
{
    return current->gForward + (*epsilon) * hvalueForward(current, env);
}

//@function     arastar fvalue backward
float fvalueBackward(POSITION* current, Environment *env, float *epsilon)
{
    return current->gBackward + (*epsilon) * hvalueBackward(current, env);
}

//@function     second function of arastar
short improvePath(POSITION *goal, float *epsilon, QList<POSITION *> *openlist, QHash<QString, POSITION*> *inconslist, Environment *env)
{
    QTime t;
    t.start();

    //QList<QPoint> expandlist;   // temp
    register int numberOfNodesExpanded = 0;
    QHash<QString, bool> closedlist;
    POSITION *current, *child;

#ifdef  CAUTION
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
        current = extractMinF(openlist, env);   // take the first node from the OPEN
#endif
        numberOfNodesExpanded++;
        //expandlist.push_back(current->ptMe);  // temp
        if (!closedlist.value(QString::number(current->ptMe.x())+QString::number(current->ptMe.y())))
            closedlist.insert(QString::number(current->ptMe.x())+QString::number(current->ptMe.y()), true);

        // print current
        //qDebug() << "----------------------------";
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
                if (child->gForward > current->gForward + cost(current, child))       // cost to any child is 1
                {
                    child->gForward = current->gForward + cost(current, child);
                    child->fForward = child->gForward + (*epsilon) * hvalueForward(child, env);
                    child->parentForward = current;
                    env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;

                    if (!closedlist.value(QString::number(child->ptMe.x())+QString::number(child->ptMe.y())))
                        minHeapInsertF(openlist, child, env);//insertInList(*openlist, child);
                    else
                    {
                        if (!inconslist->value(QString::number(child->ptMe.x())+QString::number((child->ptMe.y()))))
                            inconslist->insert(QString::number(child->ptMe.x())+QString::number((child->ptMe.y())), child);
                    }
                }
            }
        }

#ifdef CAUTION
        if (!openlist->size())  // openlist is empty
        {
            qDebug() << "FAILURE: openlist returned NULL";
            return 1;
        }
#endif
    }
    //qDebug() << QString::number(*epsilon) +" bounded expanded nodes:  " + QString::number(numberOfNodesExpanded);
    env->numberOfNodesExpandedForward = numberOfNodesExpanded;
    env->arastarTotalNodesExpanded += numberOfNodesExpanded;
#ifdef CAUTION
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
#endif
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
float hvalueForward(POSITION * current, Environment *env)
{
    float diffx = abs(env->ptGoalForward.x()-current->ptMe.x());
    float diffy = abs(env->ptGoalForward.y()-current->ptMe.y());
    return (diffx > diffy)?(diffy*qSqrt(2)+diffx-diffy):(diffx*qSqrt(2)+diffy-diffx);
}

//@function     find backward heuristic value: straight line distance
float hvalueBackward(POSITION * current, Environment *env)
{
    float diffx = abs(env->ptGoalBackward.x()-current->ptMe.x());
    float diffy = abs(env->ptGoalBackward.y()-current->ptMe.y());
    return (diffx > diffy)?(diffy*qSqrt(2)+diffx-diffy):(diffx*qSqrt(2)+diffy-diffx);
}

//@function     find cost between two nodes
float cost(POSITION * node1, POSITION *node2)
{
    float diffx = abs(node2->ptMe.x()-node1->ptMe.x());
    float diffy = abs(node2->ptMe.y()-node1->ptMe.y());
    return (diffx > diffy)?(diffy*qSqrt(2)+diffx-diffy):(diffx*qSqrt(2)+diffy-diffx);
}

//@function     inserts node as per priority
void insertInList(QList<POSITION*> &openlist, POSITION* current)
{
    POSITION* temp;
    register int x = 0;
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

//@function     GUI handling module
short MainWindow::guiHandler(Environment *env)
{
    //qDeleteAll(ui->groupBox->children());     // clear previous gourpBox layout

    gridlayout = new QGridLayout;
    QLabel *label1;
    //QImage myimage;

    gridlayout->setHorizontalSpacing(0);
    gridlayout->setVerticalSpacing(0);

    for(register int j=0;j<env->ptGridSize.y();++j)
    {
        for(register int i=0;i<env->ptGridSize.x();++i)
        {
            if (env->grid[i][j].pathExists)
            {
                label1 = new QLabel("AA__"+QString::number(i)+QString::number(j));
                //myimage.load("/home/peace/Projects/Qt/test4/myresources/01.jpg");
                //label1->setPixmap(QPixmap::fromImage(myimage));
                label1->text()="B";
                if (QPoint(i,j)==env->ptGoalForward)
                    label1->setStyleSheet("QLabel { background-color : black; }");// color : blue; }");
                else if (QPoint(i,j)==env->ptStartForward)
                    label1->setStyleSheet("QLabel { background-color : black; }");// color : blue; }");
                else if (env->grid[i][j].onPathForward)
                    label1->setStyleSheet("QLabel { background-color : yellow; }");// color : blue; }");
                else if (env->grid[i][j].visitedForward)
                    label1->setStyleSheet("QLabel { background-color : blue; }");// color : blue; }");
                else
                    label1->setStyleSheet("QLabel { background-color : green; }");// color : blue; }");

                gridlayout->addWidget(label1, j, i);
            }
            else
            {
                label1 = new QLabel("BB__"+QString::number(i)+QString::number(j));
                //label1->setText(QString::number(i)+QString::number(j));
                //QString str = ("/home/peace/Projects/Qt/test4/myresources/"+QString::number(i+1)+QString::number(i+1)+".jpg");
                // ////QString str = glb.grid[i][j].imagePath;
                // ////myimage.load(str);
                //label1->setPixmap(QPixmap::fromImage(myimage));
                label1->text()="A";
                label1->setStyleSheet("QLabel { background-color : red; }");// color : blue; }");

                gridlayout->addWidget(label1, j, i);
            }
        }
    }

    ui->groupBox->setLayout(gridlayout);
    //ui->groupBox->repaint();
    //QThread::sleep(5);
    return 0;
}

//@function     clear the previous groupBox elements
void MainWindow::on_btnClearDrawing_clicked()
{
    qDeleteAll(ui->groupBox->children());       // clear previous gourpBox layout
}

//@function     create a new window and display the results
void newGuiHandler(Environment *env, float *epsilon)
{
    //static int printSequence;
    //printSequence++;
    QWidget *window = new QWidget;
    //window->setWindowTitle("ep:" + QString::number(*epsilon) + " Seq: " + QString::number(printSequence));
    window->setWindowTitle("ep:" + QString::number(*epsilon));

    QGridLayout *gridlayout = new QGridLayout;

    //QLabel *label1 = new QLabel("Name:");
    //QLineEdit *txtName = new QLineEdit;

    //layout->addWidget(label1, 0, 0);
    //layout->addWidget(txtName, 0, 1);

    //window->setLayout(layout);

    //gridlayout = new QGridLayout;
    QLabel *label1;
    //QImage myimage;

    gridlayout->setHorizontalSpacing(0);
    gridlayout->setVerticalSpacing(0);

    for(register int j=0;j<env->ptGridSize.y();++j)
    {
        for(register int i=0;i<env->ptGridSize.x();++i)
        {
            if (env->grid[i][j].pathExists)
            {
                label1 = new QLabel("AA__"+QString::number(i)+QString::number(j));
                //myimage.load("/home/peace/Projects/Qt/test4/myresources/01.jpg");
                //label1->setPixmap(QPixmap::fromImage(myimage));
                label1->text()="B";
                if (QPoint(i,j)==env->ptGoalForward)
                    label1->setStyleSheet("QLabel { background-color : black; }");// color : blue; }");
                else if (QPoint(i,j)==env->ptStartForward)
                    label1->setStyleSheet("QLabel { background-color : black; }");// color : blue; }");
                else if (env->grid[i][j].onPathForward)
                {
                    label1->setStyleSheet("QLabel { background-color : yellow; }");// color : blue; }");
                    env->grid[i][j].onPathForward = false;
                }
                else if (env->grid[i][j].visitedForward)
                    label1->setStyleSheet("QLabel { background-color : blue; }");// color : blue; }");
                else
                    label1->setStyleSheet("QLabel { background-color : green; }");// color : blue; }");

                gridlayout->addWidget(label1, j, i);
            }
            else
            {
                label1 = new QLabel("BB__"+QString::number(i)+QString::number(j));
                //label1->setText(QString::number(i)+QString::number(j));
                //QString str = ("/home/peace/Projects/Qt/test4/myresources/"+QString::number(i+1)+QString::number(i+1)+".jpg");
                // ////QString str = glb.grid[i][j].imagePath;
                // ////myimage.load(str);
                //label1->setPixmap(QPixmap::fromImage(myimage));
                label1->text()="A";
                label1->setStyleSheet("QLabel { background-color : red; }");// color : blue; }");

                gridlayout->addWidget(label1, j, i);
            }
        }
    }

    //ui->groupBox->setLayout(gridlayout);

    //window->resize(QRect(QPoint(1,1), QPoint((env->ptGridSize.x())*10,(env->ptGridSize.y())*10)));
    //window->setSizePolicy();
    window->setFixedHeight(env->ptGridSize.y()*20);
    window->setFixedWidth(env->ptGridSize.x()*20);
    //window->resize(env->ptGridSize.x()*10, env->ptGridSize.y()*10);
    //gridlayout->setGeometry(QRect(QPoint(1,1), QPoint((env->ptGridSize.x())*10,(env->ptGridSize.y())*10)));

    //qDebug() << window->geometry();
    window->setLayout(gridlayout);
    window->show();
}

//@function     create a new window and display the results
void bidirGuiHandler(Environment *env, float *epsilon, int *printSequence)
{
    QWidget *window = new QWidget;
    window->setWindowTitle("ep:" + QString::number(*epsilon) + " Seq:" + QString::number(*printSequence));
    printSequence++;

    QGridLayout *gridlayout = new QGridLayout;

    //QLabel *label1 = new QLabel("Name:");
    //QLineEdit *txtName = new QLineEdit;

    //layout->addWidget(label1, 0, 0);
    //layout->addWidget(txtName, 0, 1);

    //window->setLayout(layout);

    //gridlayout = new QGridLayout;
    QLabel *label1;
    //QImage myimage;

    gridlayout->setHorizontalSpacing(0);
    gridlayout->setVerticalSpacing(0);

    /*
     * FOR THE ORDERING
     * common (black)
     * goal + start (black)
     * onPath (yellow)
     * visitedForward (blue)
     * visitedBackward (blue)
     * unvisited (green)
    */

    for(register int j=0;j<env->ptGridSize.y();++j)
    {
        for(register int i=0;i<env->ptGridSize.x();++i)
        {
            if (env->grid[i][j].pathExists)
            {
                label1 = new QLabel("AA__"+QString::number(i)+QString::number(j));
                label1->text()="B";

                if (env->grid[i][j].onPathCommon)
                {
                    label1->setStyleSheet("QLabel { background-color : magenta; }");
                    env->grid[i][j].onPathCommon = false;
                    env->grid[i][j].onPathForward = false;
                }
                else if (QPoint(i,j)==env->ptGoalForward)
                    label1->setStyleSheet("QLabel { background-color : black; }");
                else if (QPoint(i,j)==env->ptStartForward)
                    label1->setStyleSheet("QLabel { background-color : black; }");
                else if (env->grid[i][j].onPathForward)
                {
                    label1->setStyleSheet("QLabel { background-color : yellow; }");
                    env->grid[i][j].onPathForward = false;
                }
                else if (env->grid[i][j].visitedForward)
                    label1->setStyleSheet("QLabel { background-color : blue; }");
                else if (env->grid[i][j].visitedBackward)
                    label1->setStyleSheet("QLabel { background-color : blue; }");
                else
                    label1->setStyleSheet("QLabel { background-color : green; }");

                gridlayout->addWidget(label1, j, i);
            }
            else    // Obstacle blocks
            {
                label1 = new QLabel("BB__"+QString::number(i)+QString::number(j));
                label1->text()="A";
                label1->setStyleSheet("QLabel { background-color : red; }");

                gridlayout->addWidget(label1, j, i);
            }
        }
    }

    //ui->groupBox->setLayout(gridlayout);

    //window->resize(QRect(QPoint(1,1), QPoint((env->ptGridSize.x())*10,(env->ptGridSize.y())*10)));
    //window->setSizePolicy();
    window->setFixedHeight(env->ptGridSize.y()*20);
    window->setFixedWidth(env->ptGridSize.x()*20);
    //window->resize(env->ptGridSize.x()*10, env->ptGridSize.y()*10);
    //gridlayout->setGeometry(QRect(QPoint(1,1), QPoint((env->ptGridSize.x())*10,(env->ptGridSize.y())*10)));

    //qDebug() << window->geometry();
    window->setLayout(gridlayout);
    window->show();
}
















