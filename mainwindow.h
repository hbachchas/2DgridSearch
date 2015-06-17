#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers.h"
#include "structures.h"

using namespace searchEnv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    int visualSearch(QString searchType, Environment *env);       // Search module
    void genrateRandomSearchSpace();                                // Generate random serach space
    void createRandomBlockedConfigurations();                       // Generate blockwise search space
    void createRandomBlockedConfigurationsCCL();                    // Called from command line (CCL)
    void createLShapeRandomBlockedConfigurationsCCL();  // L shape blocks
    void createOutdoorBlockedConfigurationsCCL();       // outdoor configs
    void createInvertedCBlockedConfigurations();                    // Generate inverted C search space
    void comapreSearchCInvertedCCL();                                        // Called from command line (CCL)
    void comapreBlockbasedSearchCCL();                              // Called from command line  (CCL) for randomized blocked config
    void comapreARAstarBlockbasedSearchCCL();                       // Compares searches for blocked areas
    int compareSearch(QString searchType, Environment *env, int epsilon, PRINTINFORMATION *printInfo, QImage *image,
                      QString imageFilePath, int decrement);      // WA* and BiWA* search
    void callMeFromCMD();       // Public function to run private Testor button clicked
private slots:
    void on_btnSearch_clicked();

    void on_btnClearDrawing_clicked();

    void on_btnCompare_clicked();

    void on_btnTestor_clicked();

private:
    Ui::MainWindow *ui;

    QLabel **labelGrid;
    QGridLayout *gridlayout;
};

#endif // MAINWINDOW_H
