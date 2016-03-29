#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTCore>
#include <QtGui>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QThread>
#include <QTimer>;
#include <chrono>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef std::chrono::steady_clock::time_point time;

public:
    explicit MainWindow(QWidget *parent = 0);
    void displayLevelOnScreen(int levelNum);
    //void displayLevel(int levelNum);
    ~MainWindow();

private slots:
    void on_combo_Levels_currentIndexChanged(int index);

    void on_combo_RoomH_currentIndexChanged(int index);

    void on_combo_Boxes_currentIndexChanged(int index);

    void on_combo_RoomW_currentIndexChanged(int index);

    void on_combo_Difficulty_currentIndexChanged(int index);

    void on_generateButton_released();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    std::vector<QGraphicsPixmapItem> sprites;
    QGraphicsPixmapItem *sprite;
    QGraphicsPixmapItem *sprite2;

    bool display = true;

    bool roomWRemoved = false;
    bool roomHRemoved = false;
    void regenerateLevel(int lvlNum);
    QString createSolution(int lvlNum);

    QThread thread;
    QTime Time;
    QTimer timer;

    int genSeed = 0;

    QTime genTime = QTime::fromString("00:00:00:001", "hh:mm:ss:zzz");
    int lastTime = 0;

private slots:
    void changeProgressBar(float);
    void addToList(int);
    void displayLevel(int);
    void updateTimer();
    void disable3by3(QString);
    void on_actionClose_triggered();
    void on_actionNew_triggered();
    void on_actionSave_As_triggered();
    void rightClickMenu(const QPoint&);
    void stopTimer();
    void on_spin_TimeLimit_valueChanged(double arg1);
    void resetGUI();
    void displayLevelGenTime(int lvlNum);
    void displayGenSeed();


    void on_lineEdit_GeneratorSeed_textEdited(const QString &arg1);

    void on_actionAbout_SokoGenerator_triggered();

signals:
    void stopThread();
};

#endif // MAINWINDOW_H
