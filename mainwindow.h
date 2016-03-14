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

    QThread thread;
    QTimer *timer = new QTimer(this);
private slots:
    void changeProgressBar(float);
    void addToList(int);
    void displayLevel(int);
    void updateTimer(float);
    //void displayLevel(QListWidgetItem*);
    void disable3by3(QString);
    void on_actionClose_triggered();
    void on_actionNew_triggered();
    void on_actionSave_As_triggered();
    void rightClickMenu(const QPoint&);
};

#endif // MAINWINDOW_H
