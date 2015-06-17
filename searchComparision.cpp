//--------------- THIS HANDELS THE COMPARISION SEARCH ALGORITHMS ---------------//


#include "headers.h"

#define ARAstar_MIN_EPSILON 1 //5//1       // for ARA* family
#define ARAstar_MAX_EPSILON 1 //3       // for ARA* family

using namespace searchEnv;

int MY_INFINITY2 = 2000000000;   // 2 billion, we are using int so that overflow can be checked beforehand

void MainWindow::on_btnCompare_clicked()
{
    int TOTAL_FILES = 60;
    QList<int> weightList;
    weightList.append(10);
    weightList.append(5);
    weightList.append(2);
    weightList.append(1.5);
    weightList.append(1.1);
    weightList.append(1.05);
    weightList.append(1);
    PRINTINFORMATION printInfo;
    QString searchType = "WA*";
    QString line = "";

    // RESULTS: 2 folders each with 30 files each with 7 weights                    // Std. Dev and Mean are found lateron
    // RESULTS: 2 folders each with 7 weightsFiles each with 30 fileResults
    // PRINT_FORMAT:    SOLUTION_COST    RUNTIME     NODE_EXPANSION
    for (int weightCount=1; weightCount <= weightList.size(); weightCount++)
    {
        for (int fileCount=31; fileCount <= TOTAL_FILES; fileCount++)
        {
            for (int hubla=0;hubla<2;hubla++)
            {
                Environment env;
                QString inpConfigFile = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/env"
                        + QString::number(TOTAL_FILES) +"Files1/env"
                        + QString::number(fileCount) + ".cfg";

                prepareEnvironment(inpConfigFile, env);

                // Set start and goal for backward
                env.ptStartBackward = env.ptGoalForward;
                env.ptGoalBackward = env.ptStartForward;

                env.arastarTotalNodesExpanded = 0;

                if (compareSearch(searchType, &env, weightList.at(weightCount-1), &printInfo, NULL, "",1))    // Failure
                {
                    QMessageBox msgBox;
                    msgBox.setText("WA* could not find a path!\n Weight: " + QString::number(weightList.at(weightCount))
                                   + "\n Config File: " + QString::number(fileCount));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                    return;
                }

                // Free heap memory     // For Every file use fresh memory to avoid any conflict
                for(int j=0; j<env.ptGridSize.y(); j++)
                    for(int i=0; i<env.ptGridSize.x(); i++)
                        delete env.grid[i][j].link;

                FreeDynamicArray(env.grid);

                // Open folder 7
                QString filePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/results/file"
                        + QString::number(weightList.size()) + searchType + "/"
                        + QString::number(weightCount) + ".csv";

                QFile file(filePath);
                if (!file.open(QIODevice::Append | QIODevice::Text))
                {
                    qDebug() << "Error while opening the result file, Exiting... weightCount = "
                                + QString::number(weightCount) + " fileCount = "
                                + QString::number(fileCount);
                    QMessageBox msgBox;
                    msgBox.setText("Error while opening the result file, Exiting... weightCount = "
                                   + QString::number(weightCount) + " fileCount = "
                                   + QString::number(fileCount));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                    return;
                }
                QTextStream out(&file);
                line = QString::number(printInfo.solutionCost) + "," + QString::number(printInfo.runtime)
                    + "," + QString::number(printInfo.noOfExpansions);
                out << line << endl;
                line = "";
                out.flush();
                file.close();

                // Open folder 30
                /*filePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/results/file"
                        + QString::number(TOTAL_FILES) + searchType + "/"
                        + QString::number(fileCount) + ".csv";
                file.setFileName(filePath);
                if (!file.open(QIODevice::Append | QIODevice::Text))
                {
                    qDebug() << "Error while opening the result file, Exiting... weightCount = "
                                + QString::number(weightCount) + " fileCount = "
                                + QString::number(fileCount);
                    QMessageBox msgBox;
                    msgBox.setText("Error while opening the result file, Exiting... weightCount = "
                                   + QString::number(weightCount) + " fileCount = "
                                   + QString::number(fileCount));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                    return;
                }
                QTextStream out2(&file);
                line = QString::number(printInfo.solutionCost) + "," + QString::number(printInfo.runtime)
                    + "," + QString::number(printInfo.noOfExpansions);
                out2 << line << endl;
                line = "";
                out2.flush();
                file.close();*/

                // Toggle search type
                (searchType == "WA*") ? searchType = "BWA*" : searchType = "WA*";
            }
            qDebug("Weight: %d      File: %d \n", weightCount, fileCount);
        }
    }
    qDebug() << "\n--------All files written safely! :)---------";
}

//@function WA* and BiWA* bounded search
int MainWindow::compareSearch(QString searchType, Environment *env, int epsilon, PRINTINFORMATION *printInfo, QImage *image,
                              QString imageFilePath = "", int decrement = 1)
{
    //QList<QPoint> expandlist;   // temp
    if (searchType == "WA*")
    {
        QTime t;
        t.start();

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

        // Image painter
#ifndef STOPIMAGEFILECREATION
        if (image)
            oneTimeGridPainter( image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y(),
                           imageFilePath );
#endif

        MYINT previousFvalue = 0;
        while(current->ptMe != env->ptGoalForward)  // while(goal node is not expanded)
        {
#ifdef  CAUTION
            if (!(current = extractMinF(&openlist, env)))   // take the first node from the OPEN
            {
                if (image)
                    composeImageFile(image, true, "", 1);
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
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpanded);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalue = current->fForward;
#endif

            // Stopping reexpansion of nodes
            if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward)
                continue;
            else
                env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

            numberOfNodesExpanded++;

            // Paint the current pixel
#ifndef STOPIMAGEFILECREATION
            if (image)
                perPixelGridPainter(image, &(current->ptMe), false, true, imageFilePath, epsilon);
#endif

            if (current->ptMe == env->ptGoalForward)    // goal expanded
            {
                printInfo->runtime = t.elapsed();
                printInfo->solutionCost = current->gForward/1000;   // normalizing/actualizing the cost
                printInfo->noOfExpansions = numberOfNodesExpanded;
                //qDebug() << "pa=" << numberOfNodesExpanded;
#ifndef STOPIMAGEFILECREATION
                if (image)
                    perPixelGridPainter(image, &(current->ptMe), true, false, imageFilePath, epsilon);
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
                    if (!env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward)
                        child->fForward = child->gForward = MY_INFINITY2;
                    if (child->gForward > current->gForward + (int)cost(current, child))
                    {
                        child->gForward = current->gForward + cost(current, child);
                        child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                        minHeapInsertF(&openlist, child, env);
                        child->parentForward = current;
                        env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;
                    }
                }
            }
        }
        return 1;   // failure
    }
    else if( searchType == "BWA*" )
    {      
        QTime t;
        t.start();

        MYINT costIncumbentSolution = MY_INFINITY2;

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
        register int numberOfNodesExpandedForward = 0;
        //currentForward->fBackward = MY_INFINITY2;
        //currentForward->gBackward = MY_INFINITY2;

        POSITION *currentBackward = new POSITION;    // Backward start node
        currentBackward->ptMe = env->ptStartBackward;
        currentBackward->gBackward = 0;
        currentBackward->fBackward = currentBackward->gBackward + epsilon * hvalueBackward(currentBackward, env);
        currentBackward->parentBackward = NULL;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].visitedBackward = true;
        env->grid[currentBackward->ptMe.x()][currentBackward->ptMe.y()].link = currentBackward;
        QList<POSITION *> openlistBackward;    // Backward priority Queue
        minHeapInsertB(&openlistBackward, currentBackward, env);
        register int numberOfNodesExpandedBackward = 0;
        //currentBackward->fForward = MY_INFINITY2;
        //currentBackward->gForward = MY_INFINITY2;

        // Image painter
#ifndef STOPIMAGEFILECREATION
        if (image)
            oneTimeGridPainter( image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                           env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y(),
                           imageFilePath );
#endif

        MYINT previousFvalueForward = 0;
        MYINT previousFvalueBackward =0;
        while( costIncumbentSolution > qMax(openlistForward.at(0)->fForward + 0.001f, openlistBackward.at(0)->fBackward + 0.001f) )
        {
            //if (openlistForward.size() < openlistBackward.size())   // Expand Forward
            if ( numberOfNodesExpandedForward < numberOfNodesExpandedBackward )   // Expand Forward   // modifided to see how search grows
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

#ifndef STOPIMAGEFILECREATION
                // Paint the current pixel
                if (image)
                    perPixelGridPainter( image, &(current->ptMe), false, true, imageFilePath, epsilon );
#endif
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListBackward &&
                        costIncumbentSolution > current->gForward + current->gBackward)    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
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
                            child->fForward = child->gForward = MY_INFINITY2;
                        if (child->gForward > current->gForward + cost(current, child))
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
            if ( previousFvalueBackward, current->fBackward && epsilon == 1 )
            {
                qDebug() << "Assertion on non-decreasing fValue failed!";
                qDebug() << "expanded node number " + QString::number(numberOfNodesExpandedBackward);
                qDebug() << "x= " + QString::number(current->ptMe.x()) + "      y= " + QString::number(current->ptMe.y());
                return 1;
            }
            previousFvalueBackward = current->fBackward;
#endif

                numberOfNodesExpandedBackward++;

#ifndef STOPIMAGEFILECREATION
                // Paint the current pixel
                if (image)
                    perPixelGridPainter( image, &(current->ptMe), false, false, imageFilePath, epsilon );
#endif
                if (env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward &&
                        costIncumbentSolution > current->gForward + current->gBackward)    // current already expanded by backward
                {
                    costIncumbentSolution = current->gForward + current->gBackward;
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
                            child->fBackward = child->gBackward = MY_INFINITY2;
                        if (child->gBackward > current->gBackward + cost(current, child))
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
        printInfo->runtime = t.elapsed();
        printInfo->solutionCost = costIncumbentSolution/1000;
        printInfo->noOfExpansions = numberOfNodesExpandedForward + numberOfNodesExpandedBackward;

#ifdef DEBUG
        qDebug() << "forw exp nodes" << numberOfNodesExpandedForward;
        qDebug() << "back exp nodes" << numberOfNodesExpandedBackward;
        qDebug() << "sol cost" << costIncumbentSolution;
#endif

#ifndef STOPIMAGEFILECREATION
        // Last paint call
        if (image)
            perPixelGridPainter( image, &(current->ptMe), true, false, imageFilePath, epsilon );
#endif
        return 0;
    }
    else if (searchType == "ARA*")
    {
        epsilon = ARAstar_MAX_EPSILON;
        int iterationCount = 0;

        /*if (!ui->leEpsilon->text().isEmpty())
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        if (!ui->leDecrement->text().isEmpty())
            decrement = ui->leDecrement->text().toInt(&ok);   // decrement
        */

        QList<POSITION *> openlist;    // priority Queue
        QHash<QString, POSITION*> inconslist;

        POSITION *goal = new POSITION;
        goal->ptMe = env->ptGoalForward;
        goal->parentForward = NULL;
        goal->gForward = MY_INFINITY2;
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
        if (improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]))
        {
            qDebug() << "FAILURE: returning from improvePath, before while()";
            return 1;
        }
#else
        improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]);   // also publishes the solution
#endif

        //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )

        while(epsilon > ARAstar_MIN_EPSILON)  // while(goal node is not expanded)
        {
            epsilon = epsilon - decrement;
            iterationCount++;

            // merge openlist with inconslist
            QHash<QString, POSITION*>::iterator inconsItr;
            for (inconsItr = inconslist.begin(); inconsItr != inconslist.end(); inconsItr++)
            {
                if (inconsItr.value())
                    openlist.append(inconsItr.value());
            }
            for (register int i =0; i<openlist.size(); i++)     // update f values for new epsilon
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
            inconslist.clear();

#ifdef CAUTION
            if (improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]))
            {
                qDebug() << "FAILURE: returning from improvePath";
                return 1;
            }
#else
            improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]);
#endif

            //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
        }
        return 0;   // success
    }
    else if (searchType == "oARA*")
    {
        epsilon = ARAstar_MAX_EPSILON;
        int iterationCount = 0;

        /*if (!ui->leEpsilon->text().isEmpty())
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        if (!ui->leDecrement->text().isEmpty())
            decrement = ui->leDecrement->text().toInt(&ok);   // decrement
        */

        QList<POSITION *> openlist;    // priority Queue
        QHash<QString, POSITION*> inconslist;

        POSITION *goal = new POSITION;
        goal->ptMe = env->ptGoalForward;
        goal->parentForward = NULL;
        goal->gForward = MY_INFINITY2;
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
        if (improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]))
        {
            qDebug() << "FAILURE: returning from improvePath, before while()";
            return 1;
        }
#else
        improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]);   // also publishes the solution
#endif

        //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )

        while(epsilon > ARAstar_MIN_EPSILON)  // while(goal node is not expanded)
        {
            epsilon = epsilon - decrement;
            iterationCount++;

            // merge openlist with inconslist
            QHash<QString, POSITION*>::iterator inconsItr;
            for (inconsItr = inconslist.begin(); inconsItr != inconslist.end(); inconsItr++)
            {
                // WITH OPTIMIZATION 2
                if ( inconsItr.value() && !(inconsItr.value()->gForward < epsilon*hvalueBackward(inconsItr.value(), env)) )
                    openlist.append(inconsItr.value());
            }
            for (register int i =0; i<openlist.size(); i++)     // update f values for new epsilon
                openlist.at(i)->fForward = fvalueForward(openlist.at(i), env, &epsilon);
            buildMinHeapF(&openlist, env);
            inconslist.clear();

#ifdef CAUTION
            if (improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]))
            {
                qDebug() << "FAILURE: returning from improvePath";
                return 1;
            }
#else
            improvePathCCL(goal, epsilon, &openlist, &inconslist, env, &printInfo[iterationCount]);
#endif

            //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
        }
        return 0;   // success
    }
    else if (searchType == "BARA*")
    {
        epsilon = ARAstar_MAX_EPSILON;
        int iterationCount = 0;

        /*if (!ui->leEpsilon->text().isEmpty())
            epsilon = ui->leEpsilon->text().toInt(&ok);      // weight
        if (!ui->leDecrement->text().isEmpty())
            decrement = ui->leDecrement->text().toInt(&ok);   // decrement
        */

        QList<POSITION *> openlistForward;    // priority Queue
        QHash<QString, POSITION*> inconslistForward;

        /*POSITION *goalForward = new POSITION;
        goalForward->ptMe = env->ptGoalForward;
        goalForward->parentForward = NULL;
        goalForward->gForward = MY_INFINITY2;    // single most important line
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
        goalBackward->gBackward = MY_INFINITY2;
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
        if (improvePathBidirCCL(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env, &printInfo[iterationCount]))
        {
            qDebug() << "FAILURE: returning from improvePath, before while()";
            return 1;
        }
#else
        improvePathBidirCCL(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env, &printInfo[iterationCount]);   // also publishes the solution
#endif

        //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )

        while(epsilon > ARAstar_MIN_EPSILON)  // while(goal node is not expanded)
        {
            epsilon = epsilon - decrement;
            iterationCount++;

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
            for (register int i =0; i<openlistForward.size(); i++)
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
            for (register int i =0; i<openlistBackward.size(); i++)
                openlistBackward.at(i)->fBackward = fvalueBackward(openlistBackward.at(i), env, &epsilon);
            buildMinHeapB(&openlistBackward, env);
            inconslistBackward.clear();

#ifdef CAUTION
            if (improvePathBidirCCL(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env, &printInfo[iterationCount]))
            {
                qDebug() << "FAILURE: returning from improvePath";
                return 1;
            }
#else
            improvePathBidirCCL(epsilon, &openlistForward, &inconslistForward, &openlistBackward, &inconslistBackward, env, &printInfo[iterationCount]);
#endif

            //epsilon = epsilon < (env.grid[env.ptGoalForward.x()][env.ptGoalForward.y()].g / )
        }
        return 0;   // success
    }
    return 1;
}

void MainWindow::comapreSearchCInvertedCCL()
{
    PRINTINFORMATION printInfo;
    QString configFile = QCoreApplication::arguments().at(1);
    //QString configFile = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/invC/configFiles/env1.cfg";
    QString distanceFilePath = QCoreApplication::arguments().at(2);
    //QString distanceFilePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/invC/results/distancewise/50/";
    QString weightFilePath = QCoreApplication::arguments().at(3);
    //QString weightFilePath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/invC/results/weightwise/w1/";
    QString imageFolderPath = QCoreApplication::arguments().at(5);
    //QString imageFolderPath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/invC/results/images/";

    QString searchType = "WA*";
    bool ok;
    QString line = "";
    //int epsilon = 5;
    int epsilon = QCoreApplication::arguments().at(4).toInt(&ok);   // 1;
    //int distance = 100;
    int distance = QCoreApplication::arguments().at(6).toInt(&ok);   // 50;
    //QString baseFolderPath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/invC/";
    QString baseFolderPath = QCoreApplication::arguments().at(7);   // base path

    // PRINT_FORMAT:    SOLUTION_COST    RUNTIME     NODE_EXPANSION
    for (int turns=0;turns<2;turns++)
    {
#ifdef DEBUG
    qDebug() << configFile;
    //qDebug() << distanceFile;
    //qDebug() << weightFile;
    qDebug() << searchType;
    qDebug() << epsilon;
    qDebug() << imageFolderPath;
#endif
        Environment env;
        prepareEnvironment(configFile, env);

        // Set start and goal for backward
        env.ptStartBackward = env.ptGoalForward;
        env.ptGoalBackward = env.ptStartForward;

        env.arastarTotalNodesExpanded = 0;

        // Draw one time image
        QImage image(env.ptGridSize.y(), env.ptGridSize.x(), QImage::Format_RGB32);
        QString imageFilePath = "";
        if ( searchType == "WA*" )
            imageFilePath = imageFolderPath;// + "a/";
        else
            imageFilePath = imageFolderPath;// + "b/";

        if ( compareSearch( searchType, &env, epsilon, &printInfo, &image, imageFilePath ) )    // Failure
        {
            qDebug() << "\nSearch " + searchType + " failed!";
            qDebug("\nFailure: could not find a path! epsiolon= %f \n", epsilon );
            qDebug() << configFile;
            qDebug() << distanceFilePath;
            qDebug() << weightFilePath;
            qDebug() << epsilon;
            qDebug() << distance;
            qDebug() << imageFolderPath;
            return;
        }

        // Free heap memory     // For Every file use fresh memory to avoid any conflict
        for(int j=0; j<env.ptGridSize.y(); j++)
            for(int i=0; i<env.ptGridSize.x(); i++)
                delete env.grid[i][j].link;

        FreeDynamicArray(env.grid);

/*        // Writing to a single file
        QFile file(baseFolderPath + "results/result.dat");
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: " + distanceFilePath;
            return;
        }
        QTextStream out1(&file);
        line = "distance: " + QString::number(distance) + "    " + "weight: " + QString::number(epsilon)
                + "     " + "searchType: " + searchType;
        out1 << line << endl;
        line = QString::number(printInfo.solutionCost) + "," + QString::number(printInfo.runtime)
                + "," + QString::number(printInfo.noOfExpansions) + "," + ","  + ",";
        out1 << line << endl << endl << endl << endl;
        line = "";
        out1.flush();
        file.close();*/

        QString fileName;
        // Writing to a path file
        if (searchType=="WA*")
            fileName = "pa" + QString::number(epsilon);
        else
            fileName = "pb" + QString::number(epsilon);
        QFile file(baseFolderPath + "results/res_data/" + fileName + ".mat");
        if ( !file.exists() )
        {
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: " + distanceFilePath;
                return;
            }
            QTextStream out(&file);
            line = "# Created by Octave 3.8.1, Fri Apr 03 02:26:59 2015 IST <peace@himanshulin>";
            out << line << endl;
            line = "# name: " + fileName;
            out << line << endl;
            line = "# type: matrix";
            out << line << endl;
            line = "# rows: 1";
            out << line << endl;
            line = "# columns: 6";
            out << line << endl;
            line = "";
            out.flush();
            file.close();
        }
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: " + distanceFilePath;
            return;
        }
        QTextStream out1(&file);
        line = " " + QString::number(printInfo.solutionCost);
        out1 << line;
        if (distance == 600)
            out1 << endl;
        line = "";
        out1.flush();
        file.close();

        // Writing to a time file
        if (searchType=="WA*")
            fileName = "ta" + QString::number(epsilon);
        else
            fileName = "tb" + QString::number(epsilon);
        file.setFileName(baseFolderPath + "results/res_data/" + fileName + ".mat");
        if ( !file.exists() )
        {
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: " + distanceFilePath;
                return;
            }
            QTextStream out(&file);
            line = "# Created by Octave 3.8.1, Fri Apr 03 02:26:59 2015 IST <peace@himanshulin>";
            out << line << endl;
            line = "# name: " + fileName;
            out << line << endl;
            line = "# type: matrix";
            out << line << endl;
            line = "# rows: 1";
            out << line << endl;
            line = "# columns: 6";
            out << line << endl;
            line = "";
            out.flush();
            file.close();
        }
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: " + distanceFilePath;
            return;
        }
        QTextStream out2(&file);
        line = " " + QString::number(printInfo.runtime);
        out2 << line;
        line = "";
        out2.flush();
        file.close();

        // Writing to a expansion file
        if (searchType=="WA*")
            fileName = "ea" + QString::number(epsilon);
        else
            fileName = "eb" + QString::number(epsilon);
        file.setFileName(baseFolderPath + "results/res_data/" + fileName + ".mat");
        if ( !file.exists() )
        {
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: " + distanceFilePath;
                return;
            }
            QTextStream out(&file);
            line = "# Created by Octave 3.8.1, Fri Apr 03 02:26:59 2015 IST <peace@himanshulin>";
            out << line << endl;
            line = "# name: " + fileName;
            out << line << endl;
            line = "# type: matrix";
            out << line << endl;
            line = "# rows: 1";
            out << line << endl;
            line = "# columns: 6";
            out << line << endl;
            line = "";
            out.flush();
            file.close();
        }
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: " + distanceFilePath;
            return;
        }
        QTextStream out3(&file);
        line = " " + QString::number(printInfo.noOfExpansions);
        out3 << line;
        line = "";
        out3.flush();
        file.close();

        // Toggle search type
        (searchType == "WA*") ? searchType = "BWA*" : searchType = "WA*";
    }
}

//@function Compares searches for blocked areas
void MainWindow::comapreBlockbasedSearchCCL()
{
    // configFile  resultFile saveImagePath blockingPercentage weight fileCounter
    PRINTINFORMATION printInfo;
    QString configFile = QCoreApplication::arguments().at(1);
    QString resultPath = QCoreApplication::arguments().at(2);
    QString imageFolderPath = QCoreApplication::arguments().at(3);

    QString searchType = "WA*";
    bool ok;
    QString line = "";
    int blockPercent = QCoreApplication::arguments().at(4).toInt(&ok);
    int epsilon = QCoreApplication::arguments().at(5).toInt(&ok);
    int fileNumber = QCoreApplication::arguments().at(6).toInt(&ok);

    // PRINT_FORMAT:    SOLUTION_COST    RUNTIME     NODE_EXPANSION
    for (int turns=0;turns<2;turns++)
    {
#ifdef DEBUG
    qDebug() << configFile;
    //qDebug() << distanceFile;
    //qDebug() << weightFile;
    qDebug() << searchType;
    qDebug() << epsilon;
    qDebug() << imageFolderPath;
#endif
        Environment env;
        prepareEnvironment(configFile, env);

        // Set start and goal for backward
        env.ptStartBackward = env.ptGoalForward;
        env.ptGoalBackward = env.ptStartForward;

        env.arastarTotalNodesExpanded = 0;

        // Draw one time image
        QImage image(env.ptGridSize.y(), env.ptGridSize.x(), QImage::Format_RGB32);
        QString imageFilePath = "";
        if ( searchType == "WA*" )
            imageFilePath = imageFolderPath;// + "a/";
        else
            imageFilePath = imageFolderPath;// + "b/";

        if ( compareSearch( searchType, &env, epsilon, &printInfo, &image, imageFilePath ) )    // Failure
        {
            qDebug() << "\nSearch " + searchType + " failed!";
            qDebug("\nFailure: could not find a path! epsiolon= %f \n", epsilon );
            qDebug() << configFile;
            //qDebug() << distanceFilePath;
            qDebug() << blockPercent;
            qDebug() << epsilon;
            qDebug() << imageFolderPath;
            return;
        }

        // Free heap memory     // For Every file use fresh memory to avoid any conflict
        for(int j=0; j<env.ptGridSize.y(); j++)
            for(int i=0; i<env.ptGridSize.x(); i++)
                delete env.grid[i][j].link;

        FreeDynamicArray(env.grid);

        //if ( searchType == "WA*" )
/*        if (1)  // both are written one after another
        {
            // Open distanceFile
            //QFile file(distanceFilePath + "d.csv");
            QFile file(resultFile + "d.csv");
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                //qDebug() << "Error while opening the result file: " + distanceFilePath;
                return;
            }
            QTextStream out1(&file);
            line = QString::number(printInfo.solutionCost) + "," + QString::number(printInfo.runtime)
                    + "," + QString::number(printInfo.noOfExpansions) + "," + ","  + ",";
            out1 << line << endl;
            line = "";
            out1.flush();
            file.close();

            // Open weightFile
            file.setFileName(weightFilePath + "w.csv");
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: " + weightFilePath;
                return;
            }
            QTextStream out2(&file);
            line = QString::number(printInfo.solutionCost) + "," + QString::number(printInfo.runtime)
                    + "," + QString::number(printInfo.noOfExpansions) + "," + ","  + ",";
            out2 << line << endl;
            line = "";
            out2.flush();
            file.close();
        }
        else    // case: BWA*
        {
            // Open distanceFile
            //QFile file(distanceFilePath + "b_d.csv");
            QFile file(resultFile + "b_d.csv");
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                //qDebug() << "Error while opening the result file: " + distanceFilePath;
                return;
            }
            QTextStream out1(&file);
            line = QString::number(printInfo.solutionCost) + "," + QString::number(printInfo.runtime)
                    + "," + QString::number(printInfo.noOfExpansions) + "," + ","  + ",";
            out1 << line << endl;
            line = "";
            out1.flush();
            file.close();

            // Open weightFile
            file.setFileName(weightFilePath + "b_w.csv");
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: " + weightFilePath;
                return;
            }
            QTextStream out2(&file);
            line = QString::number(printInfo.solutionCost) + "," + QString::number(printInfo.runtime)
                    + "," + QString::number(printInfo.noOfExpansions) + "," + ","  + ",";
            out2 << line << endl;
            line = "";
            out2.flush();
            file.close();
        }
*/

        /*
        // Writing to expansion file
        QString resultFile = resultPath + "result" + QString::number(epsilon) + "_" + searchType + "_exp.csv";
        QFile file(resultFile);
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: " + resultFile;
            return;
        }
        QTextStream out1(&file);
        line = QString::number(printInfo.noOfExpansions);
        out1 << line << endl;
        line = "";
        out1.flush();
        file.close();
        // Writing to path file
        resultFile = resultPath + "result" + QString::number(epsilon) + "_" + searchType + "_path.csv";
        file.setFileName(resultFile);
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: " + resultFile;
            return;
        }
        QTextStream out2(&file);
        line = QString::number(printInfo.solutionCost);
        out2 << line << endl;
        line = "";
        out2.flush();
        file.close();
        // Writing to time file
        resultFile = resultPath + "result" + QString::number(epsilon) + "_" + searchType + "_time.csv";
        file.setFileName(resultFile);
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: " + resultFile;
            return;
        }
        QTextStream out3(&file);
        line = QString::number(printInfo.runtime);
        out3 << line << endl;
        line = "";
        out3.flush();
        file.close();*/


        QString fileName;
        // Writing to a path file
        if (searchType=="WA*")
            fileName = "pa" + QString::number(epsilon);
        else
            fileName = "pb" + QString::number(epsilon);
        QFile file(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return;
        }
        QTextStream out1(&file);
        line = " " + QString::number(printInfo.solutionCost);
        out1 << line << endl;
        line = "";
        out1.flush();
        file.close();

        // Writing to a time file
        if (searchType=="WA*")
            fileName = "ta" + QString::number(epsilon);
        else
            fileName = "tb" + QString::number(epsilon);
        file.setFileName(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return;
        }
        QTextStream out2(&file);
        line = " " + QString::number(printInfo.runtime);
        out2 << line << endl;
        line = "";
        out2.flush();
        file.close();

        // Writing to a expansion file
        if (searchType=="WA*")
            fileName = "ea" + QString::number(epsilon);
        else
            fileName = "eb" + QString::number(epsilon);
        file.setFileName(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return;
        }
        QTextStream out3(&file);
        line = " " + QString::number(printInfo.noOfExpansions);
        out3 << line << endl;
        line = "";
        out3.flush();
        file.close();


        // Toggle search type
        (searchType == "WA*") ? searchType = "BWA*" : searchType = "WA*";
    }
}

int getMean(QList<int> tempValuesList)
{
    MYINT count = 0;
    MYINT sum=0;
    for (;tempValuesList.size();)
    {
        sum += tempValuesList.takeFirst();
        count++;
    }
    if (count!=40)
        qDebug() << "Error: some value is missing, total is not 40 somewhere!";
    return (sum/count);
}

int getStdDev(MYINT mean, QList<int> tempValuesList)
{
    MYINT count = 0;
    MYINT sum=0;
    for (;tempValuesList.size();)
    {
        sum += qPow( (tempValuesList.takeFirst()-mean),2 );
        count++;
    }
    if (count!=40)
        qDebug() << "Error: some value is missing, total is not 40 somewhere!";
    count--;
    return ( qSqrt(sum/count) );
}

void findMeanStdDev()
{
#define BLOCKPERCENT_COUNT 3
#define WEIGHT_COUNT 4
#define NUMBER_OF_SEARCH_ALGORITHMS 3   // ARA* oARA* BARA*
    // from folder 3 to 12
    //QString resultPath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/ARAstarresults/resultsIndoor/res_data/";
    QString resultPath = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/ARAstarresults/resultsOutdoor/res_data/";
    int blockPercent[BLOCKPERCENT_COUNT] = {10, 20, 30};   //{10, 20, 30};   //{2, 5, 8, 12};
    int epsilon[WEIGHT_COUNT] = {5, 10, 15, 20};
    QList<QString> valueType;   valueType.append("p");valueType.append("t");valueType.append("e");
    QList<QString> searchTypeList; searchTypeList << "ARA*" << "oARA*" << "BARA*";

    for (int blocking=0; blocking<BLOCKPERCENT_COUNT; blocking++)
    {
        for (int weight=0; weight<WEIGHT_COUNT; weight++)
        {
            QString searchType = "ARA*";
            for (int j=0; j<NUMBER_OF_SEARCH_ALGORITHMS; j++)     // searchType
            {
                for (int valType=0; valType<3; valType++)   // path time expansion
                {
                    searchType = searchTypeList.at(j);  // assign search algorithm

                    if ( weight == 3 && valType == 0 )  // for p_20
                        continue;   // skip these files

                    QList<int> tempValuesList;
                    int mean = -1, stddev = -1;
                    bool ok;
                    QString line = "";

                    QString fileName;
                    // READ FILE
                    if (searchType=="ARA*")
                        fileName = valueType.at(valType) + "a" + QString::number(epsilon[weight]);
                    else if (searchType=="oARA*")
                        fileName = valueType.at(valType) + "b" + QString::number(epsilon[weight]);
                    else if (searchType=="BARA*")
                        fileName = valueType.at(valType) + "c" + QString::number(epsilon[weight]);

                    QFile file(resultPath + QString::number(blockPercent[blocking]) + "/" + fileName + ".csv");
                    qDebug() << resultPath + QString::number(blockPercent[blocking]) + "/" + fileName + ".csv";
                    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    {
                        qDebug() << "Error while opening the result file: for reading";
                        return;
                    }
                    QTextStream in(&file);
                    QStringList spaceSplitted;
                    QRegularExpression respace(" ");
                    // read line
                    for (int x=0; x<40; x++)
                    {
                        line = in.readLine();
                        spaceSplitted = line.split(respace);
                        int tempNum = spaceSplitted.last().toInt(&ok);
                        if (tempNum==2000000000)
                            tempNum = 0;
                        tempValuesList.append(tempNum);
                    }
                    file.close();

                    mean = getMean(tempValuesList);
                    stddev = getStdDev(mean, tempValuesList);

                    // WRITE MEAN
                    file.setFileName(resultPath + "mean/" + fileName + ".mat");
                    if ( !file.exists() )
                    {
                        if (!file.open(QIODevice::Append | QIODevice::Text))
                        {
                            qDebug() << "Error while opening the result file: ";
                            return;
                        }
                        QTextStream out(&file);
                        line = "# Created by Octave 3.8.1, Fri Apr 03 02:26:59 2015 IST <peace@himanshulin>";
                        out << line << endl;
                        line = "# name: " + fileName;
                        out << line << endl;
                        line = "# type: matrix";
                        out << line << endl;
                        line = "# rows: 1";
                        out << line << endl;
                        line = "# columns: " + QString::number(BLOCKPERCENT_COUNT);
                        out << line << endl;
                        line = "";
                        out.flush();
                        file.close();
                    }
                    if (!file.open(QIODevice::Append | QIODevice::Text))
                    {
                        qDebug() << "Error while opening the result file: ";
                        return;
                    }
                    QTextStream out1(&file);
                    line = " " + QString::number(mean);
                    out1 << line;
                    line = "";
                    out1.flush();
                    file.close();

                    // WRITE STD DEV
                    file.setFileName(resultPath + "stddev/" + fileName + ".mat");
                    if ( !file.exists() )
                    {
                        if (!file.open(QIODevice::Append | QIODevice::Text))
                        {
                            qDebug() << "Error while opening the result file: ";
                            return;
                        }
                        QTextStream out(&file);
                        line = "# Created by Octave 3.8.1, Fri Apr 03 02:26:59 2015 IST <peace@himanshulin>";
                        out << line << endl;
                        line = "# name: " + fileName;
                        out << line << endl;
                        line = "# type: matrix";
                        out << line << endl;
                        line = "# rows: 1";
                        out << line << endl;
                        line = "# columns: " + QString::number(BLOCKPERCENT_COUNT);
                        out << line << endl;
                        line = "";
                        out.flush();
                        file.close();
                    }
                    if (!file.open(QIODevice::Append | QIODevice::Text))
                    {
                        qDebug() << "Error while opening the result file: ";
                        return;
                    }
                    QTextStream out2(&file);
                    line = " " + QString::number(stddev);
                    out2 << line;
                    line = "";
                    out2.flush();
                    file.close();
                }


                // Toggle search type
                /*if ( searchType == "ARA*" )
                    searchType = "oARA*";
                else if ( searchType == "oARA*" )
                    searchType = "BARA*";
                else if ( searchType == "BARA*" )
                    searchType = "ARA*";*/
            }
        }
        qDebug() << "Blocking " + QString::number(blockPercent[blocking]) + " written!";
    }
    qDebug() << "All files written safely!";
}

//@function     second function of ARA*
int improvePathCCL(POSITION *goal, int epsilon, QList<POSITION *> *openlist, QHash<QString, POSITION*> *inconslist,
                   Environment *env, PRINTINFORMATION *printInfo)
{
    QTime t;
    t.start();

    //QList<QPoint> expandlist;   // temp
    int numberOfNodesExpandedForward = 0;
    //QHash<QString, bool> closedlist;
    POSITION *current=NULL, *child=NULL;
    MYINT previousFvalue = 0;

    // Draw one time image
    /*QImage *image;
    if (!globalChkZoomIN && !globalChkZoomOUT)
        image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
    else
        image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid

    oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                       env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());*/

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
            //composeImageFile(image, true);
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
        numberOfNodesExpandedForward++;

        // Paint the current pixel
/*#ifndef STOPIMAGEFILECREATION
        perPixelGridPainter(image, &(current->ptMe), false, true);
#endif*/

        env->grid[current->ptMe.x()][current->ptMe.y()].onClosedListForward = true;

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
                    child->gForward = MY_INFINITY2;
                if ( child->gForward > current->gForward + cost(current, child) )       // cost to any child is 1
                {
                    child->gForward = current->gForward + cost(current, child);
                    child->fForward = child->gForward + epsilon * hvalueForward(child, env);
                    child->parentForward = current;
                    env->grid[child->ptMe.x()][child->ptMe.y()].visitedForward = true;

                    if ( !env->grid[child->ptMe.x()][child->ptMe.y()].onClosedListForward )
                        minHeapInsertF(openlist, child, env);//insertInList(*openlist, child);
                    else    // insert into INCONS LIST
                    {
                        if ( !inconslist->value(QString::number(child->ptMe.x())+QString::number((child->ptMe.y()))) )
                            inconslist->insert(QString::number(child->ptMe.x())+QString::number((child->ptMe.y())), child);
                    }
                }
            }
        }
    }
    MYINT timeElapsed = t.elapsed();

    env->arastarTotalNodesExpanded += numberOfNodesExpandedForward;
    env->arastarTotalTimeTaken += timeElapsed;

    printInfo->noOfExpansions = numberOfNodesExpandedForward;
    printInfo->solutionCost = goal->gForward/1000;    // normalizing the cost
    printInfo->runtime = timeElapsed;

/*#ifndef STOPIMAGEFILECREATION
    if (!current)
        perPixelGridPainter(image, &(env->ptGoalForward), true, false);
    else
        perPixelGridPainter(image, &(current->ptMe), true, false);
#endif*/

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
int improvePathBidirCCL(int epsilon, QList<POSITION *> *openlistForward, QHash<QString, POSITION*> *inconslistForward,
                     QList<POSITION *> *openlistBackward, QHash<QString, POSITION*> *inconslistBackward, Environment *env, PRINTINFORMATION *printInfo)
{
    QTime t;
    t.start();

    static MYINT costIncumbentSolution = MY_INFINITY2;

    //QList<QPoint> expandlist;   // temp
    int numberOfNodesExpandedForward = 0;
    int numberOfNodesExpandedBackward = 0;
    POSITION *current=NULL, *child=NULL;
    MYINT previousFvalueForward = 0;
    MYINT previousFvalueBackward = 0;

    // Draw one time image
    /*QImage *image;
    if (!globalChkZoomIN && !globalChkZoomOUT)
        image = new QImage(env->ptGridSize.y(), env->ptGridSize.x(), QImage::Format_RGB32);     // for square grid
    else
        image = new QImage(env->ptGridSize.x(), env->ptGridSize.y(), QImage::Format_RGB32);     // for rect grid

    oneTimeGridPainter(image, env->grid, env->ptGridSize.x(), env->ptGridSize.y(),
                       env->ptStartForward.x(), env->ptStartForward.y(), env->ptGoalForward.x(), env->ptGoalForward.y());*/

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
                    child->gForward = MY_INFINITY2;
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
            //perPixelGridPainter(image, &(current->ptMe), false, true);

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
                        child->fForward = child->gForward = MY_INFINITY2;
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
            //perPixelGridPainter(image, &(current->ptMe), false, false);

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
                        child->fBackward = child->gBackward = MY_INFINITY2;
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
    MYINT timeElapsed = t.elapsed();

    env->arastarTotalNodesExpanded += numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
    env->arastarTotalTimeTaken += timeElapsed;

    printInfo->noOfExpansions = numberOfNodesExpandedForward + numberOfNodesExpandedBackward;
    printInfo->solutionCost = costIncumbentSolution/1000;    // normalizing the cost
    printInfo->runtime = timeElapsed;

    return 0;
}

//@function Compares searches for blocked areas
void MainWindow::comapreARAstarBlockbasedSearchCCL()
{
    bool ok;
    int decrement = 5, maxWeight = ARAstar_MAX_EPSILON, NUMBER_OF_SEARCHES = 3;
    //int decrement = 1, maxWeight = ARAstar_MAX_EPSILON, NUMBER_OF_SEARCHES = 3;

    PRINTINFORMATION printInfo[3];  // for number of ARA* executions
    //QString configFile = QCoreApplication::arguments().at(1);
    QString configFile = "/home/peace/Projects/Qt/searchalgos/biDIR/simpleBidirAstar/simplebidir/resources/Exp2/percent/blockP12/configFiles/env19.cfg";
    //QString resultPath = QCoreApplication::arguments().at(2);
    QString resultPath = "/home/peace/testResults/";
    QString imageFolderPath = "";   //QCoreApplication::arguments().at(3);

    QString searchType = "ARA*";
    QString line = "";
    //int blockPercent = QCoreApplication::arguments().at(3).toInt(&ok);
    int blockPercent = 12;
    int epsilon = -100; //= QCoreApplication::arguments().at(5).toInt(&ok);
    QList<int> epsilonList; //epsilonList << 15 << 10 << 5;
    for ( int i=ARAstar_MIN_EPSILON,j=ARAstar_MAX_EPSILON; j>=i; j -= decrement )
        epsilonList.append(j);

    // PRINT_FORMAT:    SOLUTION_COST    RUNTIME     NODE_EXPANSION
    for (int turns=0;turns<NUMBER_OF_SEARCHES;turns++)
    {
#ifdef DEBUG
    qDebug() << configFile;
    //qDebug() << distanceFile;
    //qDebug() << weightFile;
    qDebug() << searchType;
    qDebug() << epsilon;
    qDebug() << imageFolderPath;
#endif
        Environment env;
        if (prepareEnvironment(configFile, env))
        {
            qDebug() << "Error in prepareEnvironment, Exiting...";
            return;
        }

        // Set start and goal for backward
        env.ptStartBackward = env.ptGoalForward;
        env.ptGoalBackward = env.ptStartForward;

        env.arastarTotalNodesExpanded = 0;
        env.arastarTotalTimeTaken = 0;

        // Draw one time image
        QImage image(env.ptGridSize.y(), env.ptGridSize.x(), QImage::Format_RGB32);
        QString imageFilePath = "";
        if ( searchType == "ARA*" )
            imageFilePath = imageFolderPath;// + "a/";
        else if ( searchType == "oARA*" )
            imageFilePath = imageFolderPath;// + "b/";
        else if ( searchType == "BARA*" )
            imageFilePath = imageFolderPath;// + "c/";

        if ( compareSearch( searchType, &env, epsilon, printInfo, &image, imageFilePath, decrement ) )    // Failure
        {
            qDebug() << "\nSearch " + searchType + " failed!";
            qDebug("\nFailure: could not find a path! epsiolon= %f \n", epsilon );
            qDebug() << configFile;
            //qDebug() << distanceFilePath;
            qDebug() << blockPercent;
            qDebug() << epsilon;
            qDebug() << imageFolderPath;
            return;
        }

        // Free heap memory     // For Every file use fresh memory to avoid any conflict
        for(int j=0; j<env.ptGridSize.y(); j++)
            for(int i=0; i<env.ptGridSize.x(); i++)
                delete env.grid[i][j].link;

        FreeDynamicArray(env.grid);

        // we assume that, after this execution, we will have (e|t|p for 3 wts. and TotalExp, TotalTime for one call
        // 3 calls for weights are brought from script.sh to here

        for (int epscount = 0; epscount<epsilonList.size(); epscount++)
        {
            epsilon = epsilonList.at(epscount);

            QString fileName;
            // Writing to a path file
            if ( searchType == "ARA*" )
                fileName = "pa" + QString::number(epsilon);
            else if ( searchType == "oARA*" )
                fileName = "pb" + QString::number(epsilon);
            else if ( searchType == "BARA*" )
                fileName = "pc" + QString::number(epsilon);

            /*qDebug() << "this is first result file path";
            qDebug() << resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv";*/

            QFile file(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: from comapreARAstarBlockbasedSearchCCL";
                return;
            }
            QTextStream out1(&file);
            line = " " + QString::number(printInfo[epscount].solutionCost);
            out1 << line << endl;
            line = "";
            out1.flush();
            file.close();

            // Writing to a time file
            if ( searchType == "ARA*" )
                fileName = "ta" + QString::number(epsilon);
            else if ( searchType == "oARA*" )
                fileName = "tb" + QString::number(epsilon);
            else if ( searchType == "BARA*" )
                fileName = "tc" + QString::number(epsilon);

            file.setFileName(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: ";
                return;
            }
            QTextStream out2(&file);
            line = " " + QString::number(printInfo[epscount].runtime);
            out2 << line << endl;
            line = "";
            out2.flush();
            file.close();

            // Writing to a expansion file
            if ( searchType == "ARA*" )
                fileName = "ea" + QString::number(epsilon);
            else if ( searchType == "oARA*" )
                fileName = "eb" + QString::number(epsilon);
            else if ( searchType == "BARA*" )
                fileName = "ec" + QString::number(epsilon);

            file.setFileName(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
            if (!file.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Error while opening the result file: ";
                return;
            }
            QTextStream out3(&file);
            line = " " + QString::number(printInfo[epscount].noOfExpansions);
            out3 << line << endl;
            line = "";
            out3.flush();
            file.close();
        }

        // 2 calls for average EXPANSION and TIME
        QString fileName;
        // Writing average to a exp file
        if ( searchType == "ARA*" )
            fileName = "ea" + QString::number(maxWeight+decrement);      // 20 here is average, 5 more than the actual max. weight
        else if ( searchType == "oARA*" )
            fileName = "eb" + QString::number(maxWeight+decrement);
        else if ( searchType == "BARA*" )
            fileName = "ec" + QString::number(maxWeight+decrement);

        QFile file(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return;
        }
        QTextStream out1(&file);
        line = " " + QString::number(env.arastarTotalNodesExpanded);
        out1 << line << endl;
        line = "";
        out1.flush();
        file.close();

        // Writing average to a time file
        if ( searchType == "ARA*" )
            fileName = "ta" + QString::number(maxWeight+decrement);      // 20 here is average, 5 more than the actual max. weight
        else if ( searchType == "oARA*" )
            fileName = "tb" + QString::number(maxWeight+decrement);
        else if ( searchType == "BARA*" )
            fileName = "tc" + QString::number(maxWeight+decrement);

        file.setFileName(resultPath + "res_data/" + QString::number(blockPercent) + "/" + fileName + ".csv");
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Error while opening the result file: ";
            return;
        }
        QTextStream out2(&file);
        line = " " + QString::number(env.arastarTotalTimeTaken);
        out2 << line << endl;
        line = "";
        out2.flush();
        file.close();



        // Toggle search type
        if ( searchType == "ARA*" )
            searchType = "oARA*";
        else if ( searchType == "oARA*" )
            searchType = "BARA*";
        else if ( searchType == "BARA*" )
            searchType = "ARA*";
    }
}























