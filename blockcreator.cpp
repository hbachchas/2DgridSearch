//------------------Handles creation of blocked environments------------------//

// run it with macro CAUTION being defined

#include "headers.h"


using namespace searchEnv;

//@function True if the block lies within the legible permiter
bool isPlaceable(int **grid, int startNodeX, int startNodeY, int theta)
{
    // case theta==0
    if ( startNodeY <= 50 || startNodeX <= 50 )
        return false;
    else if ( startNodeY+50 >= (999-50) || startNodeX+150 >= (999-50) )
        return false;
    // case theta==45
    else if ( startNodeY <= 50 || startNodeX <= 50+37 )    // 50+37 & not 50, to control back edge // RATIO 7/10
        return false;
    else if ( startNodeY+107 >= (999-50) || startNodeX+107 >= (999-50) )
        return false;
    // case theta==90
    else if ( startNodeY <= 50 || startNodeX <= 50 )
        return false;
    else if ( startNodeY+150 >= (999-50) || startNodeX-50 <= 50 )
        return false;
    // case theta==135
    else if ( startNodeY <= 50+37 || startNodeX <= 50+107 )    // 57 & not 50, to control back edge
        return false;
    else if ( startNodeY+107 >= (999-50) || startNodeX-107 <= 50 )
        return false;
    return true;
}

//@function True if the block lies within the legible permiter
bool isPlaceableLShape(int startNodeX, int startNodeY, int type, int xDist, int yDist)
{
    // case 0
    if ( type==0 && startNodeY <= 20 || startNodeX <= 20 )
        return false;
    else if ( type==0 && startNodeY+yDist >= (999-20) || startNodeX+xDist >= (999-20) )
        return false;
    // case 1
    else if ( type==1 && startNodeY <= 20 || startNodeX-xDist <= 20 )    // 50+37 & not 50, to control back edge // RATIO 7/10
        return false;
    else if ( type==1 && startNodeY+yDist >= (999-20) || startNodeX >= (999-20) )
        return false;
    // case 2
    else if ( type==2 && startNodeY <= 20 || startNodeX <= 20 )
        return false;
    else if ( type==2 && startNodeY+yDist >= (999-20) || startNodeX+xDist >= (999-20) )
        return false;
    // case 3
    else if ( type==3 && startNodeY <= 20 || startNodeX-xDist <= 20 )    // 57 & not 50, to control back edge
        return false;
    else if ( startNodeY+yDist >= (999-20) || startNodeX >= 999-20 )
        return false;
    return true;
}

//@function Place the block in the grid
bool placeBlock(int **grid, int startNodeX, int startNodeY, int theta)
{
    int i,j,m,n;

    switch (theta) {
    case 0:     // case theta==0
        for( i=startNodeX; i<(startNodeX+150); i++ )
            for( j=startNodeY; j<(startNodeY+50); j++ )
                grid[i][j] = 1;
        break;
    case 1:     // case theta==45
        i=startNodeX;
        j=startNodeY;
        while( i>startNodeX-37 && j<startNodeY+37 )
        {
            m = i; n = j;
            while( m<i+105 && n<j+105 )
            {
                grid[m][n] = 1;
                n++;
                grid[m][n] = 1;
                m++;
            }
            i--; j++;
        }
        break;
    case 2:     // case theta==90
        for( i=startNodeX; i>(startNodeX-50); i-- )
            for( j=startNodeY; j<(startNodeY+150); j++ )
                grid[i][j] = 1;
        break;
    case 3:     // case theta==135
        i=startNodeX;
        j=startNodeY;
        while( i>startNodeX-37 && j>startNodeY-37 )
        {
            m = i; n = j;
            while( m>i-105 && n<j+105 )
            {
                grid[m][n] = 1;
                n++;
                grid[m][n] = 1;
                m--;
            }
            i--; j--;
        }
        break;
    default:
        break;
    }
}

bool placeBlockLShape(int **grid, int startNodeX, int startNodeY, int type, int xDist, int yDist)
{
    int i,j;

    switch (type) {
    case 0:     // case 0
        for( i=startNodeX; i<(startNodeX+3); i++ )
            for( j=startNodeY; j<(startNodeY+yDist); j++ )  // 3 thickness of the strip
                grid[i][j] = 1;
        for( i=startNodeX; i<(startNodeX+xDist); i++ )
            for( j=startNodeY+yDist; j<(startNodeY+yDist+3); j++ )
                grid[i][j] = 1;
        break;
    case 1:     // case 1
        for( i=startNodeX; i<(startNodeX+3); i++ )
            for( j=startNodeY; j<(startNodeY+yDist); j++ )
                grid[i][j] = 1;
        for( i=startNodeX+3; i>(startNodeX-xDist); i-- )    // offset 3, for checking corner misses
            for( j=startNodeY+yDist; j<(startNodeY+yDist+3); j++ )
                grid[i][j] = 1;
        for( i=startNodeX; i>(startNodeX-xDist); i-- )
            for( j=startNodeY; j<(startNodeY+3); j++ )
                grid[i][j] = 1;
        break;
    case 2:     // case 2
        for( i=startNodeX; i<(startNodeX+xDist); i++ )
            for( j=startNodeY; j<(startNodeY+3); j++ )
                grid[i][j] = 1;
        for( i=startNodeX+xDist; i<(startNodeX+xDist+3); i++ )
            for( j=startNodeY; j<(startNodeY+yDist); j++ )
                grid[i][j] = 1;
        for( i=startNodeX; i<(startNodeX+xDist+3); i++ )    // offset 3, for checking corner misses
            for( j=startNodeY+yDist; j<(startNodeY+yDist+3); j++ )
                grid[i][j] = 1;
        break;
    case 3:     // case 3
        for( i=startNodeX; i>(startNodeX-xDist); i-- )
            for( j=startNodeY; j<(startNodeY+3); j++ )
                grid[i][j] = 1;
        for( i=startNodeX-xDist; i<(startNodeX-xDist+3); i++ )
            for( j=startNodeY; j<(startNodeY+yDist); j++ )
                grid[i][j] = 1;
        break;
    default:
        break;
    }
}

//@function     create QImage for every config file
void gridPainter(int **displayGrid, int startX, int startY, int goalX, int goalY, QString path, int fileCount, bool silent)
{
    QWidget *window = new QWidget;
    window->setWindowTitle("Compressed Configuration");
    QGridLayout *gridlayout = new QGridLayout;
    QLabel *label1 = new QLabel("A");

    gridlayout->setHorizontalSpacing(0);
    gridlayout->setVerticalSpacing(0);

    int IMAGE_X = 1000, IMAGE_Y = 1000;
    QImage image(IMAGE_Y, IMAGE_X, QImage::Format_RGB32);
    QRgb value;

    // FOR THE ORDERING
    // common (green)
    // goal + start (black)
    // blocked (red)

    for( int j=0; j<IMAGE_Y; j++ )
    {
        for( int i=0; i<IMAGE_X; i++ )
        {
            if ( displayGrid[i][j]==0 )
            {
                if ( i==startX && j==startY || i==goalX && j==goalY )       // start or goal states
                {
                    value = qRgb(0, 0, 0); // black
                    image.setPixel(i, j, value);
                }
                else
                {
                    value = qRgb(0, 250, 0); // green
                    image.setPixel(i, j, value);
                }
            }
            else    // Obstacle blocks
            {
                value = qRgb(250, 0, 0); // red
                image.setPixel(i, j, value);
            }
        }
    }

    // darken the start and goal nodes
    value = qRgb(0, 0, 0); // black
    image.setPixel(startX, startY, value);
    image.setPixel(startX+1, startY, value);
    image.setPixel(startX, startY+1, value);
    image.setPixel(startX+1, startY+1, value);
    image.setPixel(startX+2, startY, value);
    image.setPixel(startX+2, startY+1, value);
    image.setPixel(startX+2, startY+2, value);
    image.setPixel(startX+1, startY+2, value);
    image.setPixel(startX, startY+2, value);
    image.setPixel(goalX, goalY, value);
    image.setPixel(goalX+1, goalY, value);
    image.setPixel(goalX, goalY+1, value);
    image.setPixel(goalX+1, goalY+1, value);
    image.setPixel(goalX+2, goalY, value);
    image.setPixel(goalX+2, goalY+1, value);
    image.setPixel(goalX+2, goalY+2, value);
    image.setPixel(goalX+1, goalY+2, value);
    image.setPixel(goalX, goalY+2, value);

    QImageWriter writer( path + "image" + QString::number(fileCount) + ".png", "png" );
    writer.setText("Author", "HB");
    writer.write(image);

    label1->setPixmap( QPixmap::fromImage(image) );
    gridlayout->addWidget( label1, 0, 0 );

    window->setFixedHeight(IMAGE_Y);
    window->setFixedWidth(IMAGE_X);

    window->setLayout(gridlayout);
    if (!silent)    //  Display
        window->show();
}

//@NOTE:    run it with macro CAUTION being defined
void MainWindow::createRandomBlockedConfigurations()
{
    int TOTAL_FILES = 30;
    int **grid = AllocateDynamicArray<int>(1000, 1000);     // now plot the grid
    int blockSize[2] = {10,50}; //{x,y}
    int theta;  // angle 0 45 90 135

    // Create seed for the random   // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QList<int> blockingPercentageList;
    blockingPercentageList.append(10);blockingPercentageList.append(16);blockingPercentageList.append(22);
    blockingPercentageList.append(28);blockingPercentageList.append(34);blockingPercentageList.append(40);
    blockingPercentageList.append(46);
    int NUMBER_OF_WEIGHTS =blockingPercentageList.size();

    while(blockingPercentageList.size()>0)
    {
        //TOTAL_FILES = 30 - qPow( 1.7, NUMBER_OF_WEIGHTS-blockingPercentageList.size() );
        int blockingPercentage = blockingPercentageList.takeFirst();
        int totalBlockings = blockingPercentage * 1000 * 1000 / 100 ;

        QString configFilePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/percent/blockP" +
                QString::number(blockingPercentage) +"/configFiles/";
        QString saveImagePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/percent/blockP" +
                QString::number(blockingPercentage) +"/configImages/";

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
                theta = getRandomNumber(0,3);   // 0-0 1-45 2-90 3-135  // random orientation
                // To generate only horizontal and vertical obstacles
                /*if ( theta == 1 )
                theta = 0;
            if ( theta == 3 )
                theta = 2;*/

                // if the block doesn't hit the sides
                if (isPlaceable(grid, startNodeX, startNodeY, theta))
                {
                    placeBlock(grid, startNodeX, startNodeY, theta);    // draw the block
                    if (!(theta % 2))   // even 0 or 90
                        i = i + 10*50;
                    else                // odd 45 or 135
                        i = i + 10*35;
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
                    QMessageBox msgBox;
                    msgBox.setText( "Do you want to save it?" );
                    msgBox.setInformativeText( "Save the Document?" );
                    msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard );
                    msgBox.setDefaultButton( QMessageBox::Save );
                    //int ret = msgBox.exec();

                    //if ( ret == QMessageBox::Save )
                    if (1)
                    {
                        //call A painter function to paint 1000x1000 image and save it
                        gridPainter(grid, startNodeX, startNodeY, goalNodeX, goalNodeY, saveImagePath, fileCounter, true);
                        // write them in a file
                        composeSearchSpaceFile(grid, configFilePath, fileCounter, startNodeX, startNodeY, goalNodeX, goalNodeY);
                        qDebug() << ">>>blocking% " + QString::number(blockingPercentage) +
                                    " File " + QString::number(fileCounter) + " written!";
                        fileCounter++;
                        break;
                    }
                    else
                        break;
                }
            }
        }
    }
    FreeDynamicArray_blockcreator(grid);     // Free the grid memory
    qDebug() << ">>>--------All files written safely! :)---------";
}

//@NOTE:    run it with macro CAUTION being defined
//@function Called from command line (CCL)
void MainWindow::createRandomBlockedConfigurationsCCL()
{
    //qDebug() << "I did it"; return;
    int TOTAL_FILES = 1;
    int **grid = AllocateDynamicArray<int>(1000, 1000);     // now plot the grid
    int blockSize[2] = {10,150}; //{x,y}
    int theta;  // angle 0 45 90 135
    bool ok;

    // Create seed for the random   // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int blockingPercentage = QCoreApplication::arguments().at(3).toInt(&ok);
    int fileNumber = QCoreApplication::arguments().at(4).toInt(&ok);
    int totalBlockings = blockingPercentage * 1000 * 1000 / 100 ;

    QString configFilePath = QCoreApplication::arguments().at(1);
    QString saveImagePath = QCoreApplication::arguments().at(2);

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
            //theta = getRandomNumber(0,3);   // 0-0 1-45 2-90 3-135  // random orientation
            theta = 3;  // generate 45 degree obstacles only
            // To generate only horizontal and vertical obstacles
            /*if ( theta == 1 )
                theta = 0;
            if ( theta == 3 )
                theta = 2;*/

            // if the block doesn't hit the sides
            if (isPlaceable(grid, startNodeX, startNodeY, theta))
            {
                placeBlock(grid, startNodeX, startNodeY, theta);    // draw the block
                if (!(theta % 2))   // even 0 or 90
                    i += 50*150;
                else                // odd 45 or 135
                    i += 50*105;
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

            env.epsilon = 1;

            prepareAstarEnvironment(env, grid);

            // Call A* to find that there exists a path between those 2 nodes
            if (!visualSearch("WA*", &env))   // Path exists
            {
                //call A painter function to paint 1000x1000 image and save it
                gridPainter(grid, startNodeX, startNodeY, goalNodeX, goalNodeY, saveImagePath, fileNumber, true); // silent mode
                // write them in a file
                composeSearchSpaceFile(grid, configFilePath, fileNumber, startNodeX, startNodeY, goalNodeX, goalNodeY);
                qDebug() << ">>>blocking% " + QString::number(blockingPercentage) +
                            " File " + QString::number(fileNumber) + " written!";
                fileCounter++;
                break;
            }
        }
    }
    FreeDynamicArray_blockcreator(grid);     // Free the grid memory
}

//@NOTE:    run it with macro CAUTION being defined
void MainWindow::createLShapeRandomBlockedConfigurationsCCL()
{
    //qDebug() << "I did it"; return;
    int TOTAL_FILES = 1;
    int **grid = AllocateDynamicArray<int>(1000, 1000);     // now plot the grid
    int blockSize[2] = {300,400}; //{x,y}
    int type;  // type 0(L) 1 2 3 anti-clockwise shift of corner
    bool ok; 

    // Create seed for the random   // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int blockingPercentage = QCoreApplication::arguments().at(3).toInt(&ok);
    int fileNumber = QCoreApplication::arguments().at(4).toInt(&ok);

#ifndef CALLFROMCOMMANDLINE
    if (!ui->lePercentBlocking->text().isEmpty())
        blockingPercentage = ui->lePercentBlocking->text().toInt(&ok);      // weight
#endif

    int totalBlockings = blockingPercentage * 1000 * 1000 / 100 ;

    QString configFilePath = QCoreApplication::arguments().at(1);
    //QString configFilePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/percent/blockP5/configFiles/";
    QString saveImagePath = QCoreApplication::arguments().at(2);
    //QString saveImagePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/percent/blockP5/configImages/";

    int startNodeX, startNodeY, goalNodeX, goalNodeY;
    for (int fileCounter=1; fileCounter<=TOTAL_FILES;)
    {
        for (int i=0; i<1000; i++)      // Set all to 0
            for (int j=0; j<1000; j++)
                grid[i][j] = 0;

        // INITIALIZE THE GRID STRUCTURE
        for (int i=0; i<totalBlockings;)    // Fill the blocking in the grid
        {
            // generate a random x,y
            startNodeX = getRandomNumber(0+20,999-20);
            startNodeY = getRandomNumber(0+20,999-20);

            type = getRandomNumber(1,2);   // type 0(L) 1 2 3 anti-clockwise shift of corner  // random orientation

            // if the block doesn't hit the sides
            if (isPlaceableLShape(startNodeX, startNodeY, type, blockSize[0], blockSize[1]))
            {
                placeBlockLShape(grid, startNodeX, startNodeY, type, blockSize[0], blockSize[1]);    // draw the block
                i += 3 * (2*blockSize[0] + blockSize[1]);
            }
        }

        startNodeX=0, startNodeY=500, goalNodeX=995, goalNodeY=500;   // Reinitialize start(x,y) and goal(x,y)
        //call A painter function to paint 1000x1000 image and save it
        gridPainter(grid, startNodeX, startNodeY, goalNodeX, goalNodeY, saveImagePath, fileNumber, true); // silent mode
        // write them in a file
        composeSearchSpaceFile(grid, configFilePath, fileNumber, startNodeX, startNodeY, goalNodeX, goalNodeY);
        qDebug() << ">>>blocking% " + QString::number(blockingPercentage) +
                    " File " + QString::number(fileNumber) + " written!";
        fileCounter++;
        // FIND A PAIR OF START & GOAL NODES & WRITE THE GRID INTO A FILE
        /*while(startNodeX<250 && startNodeY<250 && goalNodeX>750 && goalNodeY>750)   // Min distance between start and goal is 500
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

            env.epsilon = 1;

            prepareAstarEnvironment(env, grid);

            // Call A* to find that there exists a path between those 2 nodes
            if (!visualSearch("WA*", &env))   // Path exists
            {
                //call A painter function to paint 1000x1000 image and save it
                gridPainter(grid, startNodeX, startNodeY, goalNodeX, goalNodeY, saveImagePath, fileNumber, true); // silent mode
                // write them in a file
                composeSearchSpaceFile(grid, configFilePath, fileNumber, startNodeX, startNodeY, goalNodeX, goalNodeY);
                qDebug() << ">>>blocking% " + QString::number(blockingPercentage) +
                            " File " + QString::number(fileNumber) + " written!";
                fileCounter++;
                break;
            }
        }
        */
    }
    FreeDynamicArray_blockcreator(grid);     // Free the grid memory
}

//@function True if the block lies within the legible permiter
bool isPlaceableOutdoorShape(int **grid, int startNodeX, int startNodeY, int type, int xDist, int yDist)
{
    for (int i=startNodeX; i<startNodeX+xDist; i++)
        for (int j=startNodeY; j<startNodeY+yDist; j++)
        {
            if (grid[i][j])
                return false;
        }
    return true;
}

bool placeBlockOutdoorShape(int **grid, int startNodeX, int startNodeY, int type, int xDist, int yDist)
{
    for (int i=startNodeX; i<startNodeX+xDist; i++)
        for (int j=startNodeY; j<startNodeY+yDist; j++)
            grid[i][j] = 1;

    return true;
}

//@NOTE:    run it with macro CAUTION being defined
void MainWindow::createOutdoorBlockedConfigurationsCCL()
{
    int TOTAL_FILES = 1;
    int **grid = AllocateDynamicArray<int>(1000, 1000);     // now plot the grid
    int blockSize[2] = {-1,-1}; //{x,y}
    int type;  // type 0-square(50x50) 1-rect(100x50) 2-(50x100)
    bool ok;

    // Create seed for the random   // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int blockingPercentage = QCoreApplication::arguments().at(3).toInt(&ok);
    //int blockingPercentage = 30;
    int fileNumber = QCoreApplication::arguments().at(4).toInt(&ok);
    //int fileNumber = 1;

#ifndef CALLFROMCOMMANDLINE
    if (!ui->lePercentBlocking->text().isEmpty())
        blockingPercentage = ui->lePercentBlocking->text().toInt(&ok);      // weight
#endif

    int totalBlockings = blockingPercentage * 1000 * 1000 / 100 ;

    QString configFilePath = QCoreApplication::arguments().at(1);
    //QString configFilePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/percent_outdoor/blockP30/configFiles/";
    QString saveImagePath = QCoreApplication::arguments().at(2);
    //QString saveImagePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/percent_outdoor/blockP30/configImages/";

    int startNodeX, startNodeY, goalNodeX, goalNodeY;
    for (int fileCounter=1; fileCounter<=TOTAL_FILES;)
    {
        for (int i=0; i<1000; i++)      // Set all to 0
            for (int j=0; j<1000; j++)
                grid[i][j] = 0;

        // INITIALIZE THE GRID STRUCTURE
        for (int i=0; i<totalBlockings;)    // Fill the blocking in the grid
        {
            type = getRandomNumber(0,2);   // type 0-square(50x50) 1-rect(100x50) 2-(50x100)
            if (type == 0)
                blockSize[0] = blockSize[1] = 50;
            else if (type == 1)
            {
                blockSize[0] = 200;
                blockSize[1] = 50;
            }
            else if (type == 2)
            {
                blockSize[0] = 50;
                blockSize[1] = 200;
            }

            // generate a random x,y
            startNodeX = getRandomNumber(0+20,999-20-blockSize[0]);
            startNodeY = getRandomNumber(0+20,999-20-blockSize[1]);

            // if the block doesn't hit the sides
            if ( isPlaceableOutdoorShape(grid, startNodeX, startNodeY, type, blockSize[0], blockSize[1]) )
            {
                placeBlockOutdoorShape(grid, startNodeX, startNodeY, type, blockSize[0], blockSize[1]);    // draw the block
                i += blockSize[0]*blockSize[1];
            }
        }

        startNodeX=0, startNodeY=400, goalNodeX=995, goalNodeY=400;   // Reinitialize start(x,y) and goal(x,y)
        //call A painter function to paint 1000x1000 image and save it
        gridPainter(grid, startNodeX, startNodeY, goalNodeX, goalNodeY, saveImagePath, fileNumber, true); // silent mode
        // write them in a file
        composeSearchSpaceFile(grid, configFilePath, fileNumber, startNodeX, startNodeY, goalNodeX, goalNodeY);
        qDebug() << ">>>blocking% " + QString::number(blockingPercentage) +
                    " File " + QString::number(fileNumber) + " written!";
        fileCounter++;
    }
    FreeDynamicArray_blockcreator(grid);     // Free the grid memory
}

//@function Draw inverted C configuration
void getInvertedCBlock(int **grid, int d)
{
    int hDistance=300, vDistance=850;
    for( int i=d+hDistance; i<d+hDistance+10; i++ )     // vertical
        for( int j=(1000-vDistance)/2; j<(1000+vDistance)/2; j++ )
            grid[i][j] = 1;
    for( int i=d; i<d+hDistance+4; i++ )     // horizontal top
        for( int j=(1000-vDistance)/2; j<(1000-vDistance)/2+10; j++ )
            grid[i][j] = 1;
    for( int i=d; i<d+hDistance+4; i++ )     // horizontal bottom
        for( int j=(1000+vDistance)/2-10; j<(1000+vDistance)/2; j++ )
            grid[i][j] = 1;
}

// DEPRECATED - no longer in use
void MainWindow::createInvertedCBlockedConfigurations()
{
    //int TOTAL_FILES = 7;
    int **grid = AllocateDynamicArray<int>(1000, 1000);     // now plot the grid
    QList<int> distanceList;
    distanceList.append(100);distanceList.append(200);distanceList.append(300);
    distanceList.append(400);distanceList.append(500);distanceList.append(600);//distanceList.append(650);

    QString filePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/invC/configFiles/";
    QString saveImagePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/invC/configImages/";

    int startNodeX=0, startNodeY=500, goalNodeX=995, goalNodeY=500;
    for ( int fileCounter=1; distanceList.size()>0; fileCounter++ )
    {
        for (int i=0; i<1000; i++)      // Set all to 0
            for (int j=0; j<1000; j++)
                grid[i][j] = 0;
        //grid[startNodeX][startNodeY] = 1;
        //grid[goalNodeX][goalNodeY] = 1;

        // INITIALIZE THE GRID STRUCTURE
        getInvertedCBlock(grid, distanceList.takeFirst());

        //call A painter function to paint 1000x1000 image and save it
        gridPainter( grid, startNodeX, startNodeY, goalNodeX, goalNodeY, saveImagePath, fileCounter, true );
        // write them in a file
        composeSearchSpaceFile( grid, filePath, fileCounter, startNodeX, startNodeY, goalNodeX, goalNodeY );
        qDebug() << ">>>File " + QString::number(fileCounter) + " written!";
    }
    FreeDynamicArray_blockcreator(grid);     // Free the grid memory
    qDebug() << "\n>>>--------All files written safely! :)---------";
}

//@function     delete the dynamic memory allocated to a 2D array
template <typename T>
void FreeDynamicArray_blockcreator(T** dArray)
{
      delete [] *dArray;
      delete [] dArray;
}


/*
 * make a 1000x1000 array
 * percent = area to be blocked
 * block size = 10x50
 * while (the percent block area is not blocked) {
 x,y = get random coordinate()
 theta = get random orientation()
if (isPlaceable(x,y,theta): placement of the block doesn't cross the boundary [50 px inside the outer region])
    then
        place the block
        increase the blocked area
}

       find out random two points and check if the path exists between them, if yes then

scaleDown(by parameter alpha)

loop: 4 times (scale down horizontally)
    loop: j=0 to 998
        loop: i=0 to 249
            array[i][j] = array[4*i+0][j] | array[4*i+1][j] | array[4*i+2][j] | array[4*i+3][j]    // bit wise ORing

loop: 4 times (scale down)
    loop: i=0 to 249
        loop: j=0 to 249
            array[i][j] = array[i][4*j+0] | array[i][4*j+1] | array[i][4*j+2] | array[i][4*j+3]    // bit wise ORing


call A painter function to paint 250x250 image
if image good   // user decides
    then
        save it!
 * /




 /*
  * getAmirroredCBlock at a distance 'd' from left: configuration file();
  * scale down it and show it for approval
  *     if good then save it
  * /



  /*
   * getAmirroredCBlock(distance d)
   * loop: x=d+150 to d+150+10
   *    loop: y=140 to 860
   *        print array[x][y] = 1
   *
   * loop: x=d to d+151
   *    loop: y=140 to 150
   *        print array[x][y] = 1
   *
   * loop: x=d to d+151
   *    loop: y=850 to 860
   *        print array[x][y] = 1
   *
   */





