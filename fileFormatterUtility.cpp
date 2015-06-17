//--------------- THIS HANDELS THE FORMATTING OF RESULTS FROM FILES ---------------//


#include "headers.h"

using namespace searchEnv;


//@NOTE:    run it with macro CAUTION being defined
//@function     generate random serach space
void MainWindow::genrateRandomSearchSpace()
{
    int TOTAL_FILES = 1;
    float blockingPercentage = .15;
    int totalBlockings = blockingPercentage * 1000 * 1000;
    int **grid = AllocateDynamicArray<int>(1000, 1000);     // now plot the grid

    // Create seed for the random   // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int startNodeX=0, startNodeY=0, goalNodeX=999, goalNodeY=999;
    for (int fileCounter=1; fileCounter<=TOTAL_FILES;)
    {
        for (int i=0; i<1000; i++)      // Set all to 0
            for (int j=0; j<1000; j++)
                grid[i][j] = 0;

        // INITIALIZE THE GRID STRUCTURE
        for (int i=0; i<totalBlockings;)    // Fill the blocking in the grid
        {
            // generate a random x,y coordinate between 0,0 and 999,999         // take time as random seed for increased randomization
            startNodeX = getRandomNumber(0,999);
            startNodeY = getRandomNumber(0,999);

            // if x,y is not a blockade
            if (!grid[startNodeX][startNodeY])
            {
                grid[startNodeX][startNodeY] = 1;   // then make it a blockade
                i++;
            }
        }

        startNodeX=0, startNodeY=0, goalNodeX=999, goalNodeY=999;   // Reinitialize start(x,y) and goal(x,y)
        // FIND A PAIR OF START & GOAL NODES & WRITE THE GRID INTO A FILE
        while(startNodeX<250 && startNodeY<250 && goalNodeX>750 && goalNodeY>750)   // Min distance between start and goal is 500
        {
            // Generate random pair of start [(0,0),(249,249)] and goal [(751,751),(999,999)] node
            startNodeX = getRandomNumber(0,249);
            startNodeY = getRandomNumber(0,249);
            goalNodeX = getRandomNumber(751,999);
            goalNodeY = getRandomNumber(751,999);
            if (grid[startNodeX][startNodeY] || grid[goalNodeX][goalNodeY])     // if any of the node is invalid
                continue;   // then continue

            // INITIALIZE ENVIRONMENT TO CALL A*

            Environment env;    // search global variable

            // Set start and goal state
            env.ptStartForward.setX(startNodeX);
            env.ptStartForward.setY(startNodeY);
            env.ptGoalForward.setX(goalNodeX);
            env.ptGoalForward.setY(goalNodeY);

            prepareAstarEnvironment(env, grid);

            // Call A* to find that there exists a path between those 2 nodes
            if (!visualSearch("WA*", &env))   // Path exists
            {
                // write them in a file
                composeSearchSpaceFile(grid, "", fileCounter, startNodeX, startNodeY, goalNodeX, goalNodeY);
                qDebug() << "File " + QString::number(fileCounter) + " written!";
                fileCounter++;
                break;
            }
        }
    }
    //FreeDynamicArray(grid);     // Free the grid memory
    qDebug() << "\n--------All files written safely! :)---------";
}

//@function     Random number generator
int getRandomNumber(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

//@function     Write random generated search space in a FILE
int composeSearchSpaceFile(int **grid, QString path, int fileNumber, int startNodeX, int startNodeY, int goalNodeX, int goalNodeY)
{
    int MAX_X = 1000;
    int MAX_Y = 1000;
    if (path.isEmpty())
        path = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/env30Files1/";
    QString configFilePath = path + "env" + QString::number(fileNumber) + ".cfg";
    QFile file(configFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Error file opening the config file, Exiting... from composeSearchSpaceFile";
        return -1;
    }

    QTextStream out(&file);
    QString line;

    // line1
    line = "discretization(cells): " + QString::number(1000) + " " + QString::number(1000) ;
    out << line << endl;
    // line2
    line = "obsthresh: 1";
    out << line << endl;
    // line3
    line = "start(cells): " + QString::number(startNodeX) + " " + QString::number(startNodeY);
    out << line << endl;
    // line4
    line = "end(cells): " + QString::number(goalNodeX) + " " + QString::number(goalNodeY);
    out << line << endl;
    // line5
    line = "environment:";
    out << line << endl;
    // grid array
    for (int j=0; j<MAX_Y; j++)
    {
        line = "";
        for (int i=0; i<MAX_X; i++)
        {
            line = line + QString::number(grid[i][j]) + " ";
        }
        out << line << endl;
    }
    // an empty line
    line = "";
    out << line << endl;
    // the absolute path
    line = configFilePath;
    out << line << endl;

    out.flush();
    file.close();
}

//@function     Prepare environment from IN-Memory grid variable
void prepareAstarEnvironment(Environment &env, int **grid)
{
    // Set GridSize
    env.ptGridSize.setX(1000);
    env.ptGridSize.setY(1000);

    // now plot the grid
    env.grid = AllocateDynamicArray<GRIDSTATE>(env.ptGridSize.x(), env.ptGridSize.y());

    // set interior states
    for(register int j=0; j<env.ptGridSize.y(); j++)   // increment y
    {
        for(register int i=0;i<env.ptGridSize.x();i++)
        {
                env.grid[i][j].pathExists = !(grid[i][j]);
                env.grid[i][j].ptMe.setX(i);
                env.grid[i][j].ptMe.setY(j);
                env.grid[i][j].visitedForward = false;
                env.grid[i][j].heapIndexForward = -1;
                env.grid[i][j].onPathForward = false;
                env.grid[i][j].onClosedListForward = false;
                env.grid[i][j].visitedBackward = false;
                env.grid[i][j].heapIndexBackward = -1;
                env.grid[i][j].onPathBackward = false;
                env.grid[i][j].onClosedListBackward = false;
                env.grid[i][j].link = NULL;
                env.grid[i][j].onPathCommon = false;
        }
    }
}





//@function     Read two files, Merge data horizontally (side to side)
void mergeFiles()
{
    int TOTAL_FILES = 7;
    QString line = "";
    for (int fileCount=1; fileCount<=TOTAL_FILES; fileCount++)
    {
        QString filePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/results(60)/file"
                + QString::number(TOTAL_FILES) + "WA*" + "/"
                + QString::number(fileCount) + ".csv";
        QFile file1(filePath);
        qDebug() << filePath;
        if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("Error while opening the result file!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
        QTextStream in1(&file1);

        filePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/results(60)/file"
                + QString::number(TOTAL_FILES) + "BWA*" + "/"
                + QString::number(fileCount) + ".csv";
        QFile file2(filePath);
        if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("Error while opening the result file!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
        QTextStream in2(&file2);

        filePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/results(60)/file"
                + QString::number(TOTAL_FILES) + "concat" + "/"
                + QString::number(fileCount) + ".csv";
        QFile file3(filePath);
        if (!file3.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("Error while opening the result file!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
        QTextStream out(&file3);

        while(!in1.atEnd() && !in2.atEnd())
        {
            line = "," + in1.readLine() + ",," + in2.readLine();
            out << line << endl;
        }

        out.flush();
        file1.close();
        file2.close();
        file3.close();

        qDebug("File %d written!", fileCount);
    }
    qDebug() <<  "Files merged successfully!";
}
