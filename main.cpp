#include "mainwindow.h"
#include <QApplication>
#include <QTextStream>
#include <structures.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#ifndef CALLFROMCOMMANDLINE
    w.show();
#else
    if (argc<4)//5)     // configFileName  imageFileName blockingPercentage fileCounter imageFile distance_C
    {
        QTextStream(stdout) << ">>> Not enough arguments. Please enter config Path followed by image Path" << endl;
        QTextStream(stdout) << ">>> Terminating... from main" << endl;
        return 0;
    }
    else
        w.callMeFromCMD();
#endif

    return a.exec();
}


