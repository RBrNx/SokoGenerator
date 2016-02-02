#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTCore>
#include <QtGui>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void displayLevel(int levelNum);
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
    std::vector<QGraphicsPixmapItem> sprites;
    QGraphicsPixmapItem *sprite;
    QGraphicsPixmapItem *sprite2;

private slots:
    void changeProgressBar(float);
    void addToList(int);
    void displayLevel(QListWidgetItem*);
};

#endif // MAINWINDOW_H
