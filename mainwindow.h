#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTCore>
#include <QtGui>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_combo_Levels_activated(int index);

    void on_combo_RoomH_activated(int index);

    void on_combo_Boxes_activated(int index);

    void on_combo_RoomW_activated(int index);

    void on_combo_Difficulty_activated(int index);

    void on_generateButton_released();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

private slots:
    void changeProgressBar(float);
    void addToList(int);
};

#endif // MAINWINDOW_H
