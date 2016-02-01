#include "mainwindow.h"
#include "sokogenerator.h"
#include <QApplication>
#include <QProgressBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Sokoban Level Generator");
    mainWindow.show();

    return a.exec();
}
