#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sokogenerator.h"

SokoGenerator Generator;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_GenerationTime->setText("Estimated Generation Time: 0 Hours 0 Minutes");
    ui->progressBar->setValue(0);

    connect(&Generator, SIGNAL(changeProgressBar(float)), this, SLOT(changeProgressBar(float)));
    connect(&Generator, SIGNAL(addToList(int)), this, SLOT(addToList(int)));

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //QPixmap pixmap(":/tileset/textures/box.png");
    QGraphicsPixmapItem *myItem = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/box.png"));
    scene->addItem(myItem);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeProgressBar(float value){
    ui->progressBar->setValue(value);
}

void MainWindow::addToList(int value){
    ui->list_LevelSet->addItem("Level " + QString::number(value));
    QVariant dataValue(value);
    ui->list_LevelSet->item(value-1)->setData(Qt::UserRole, dataValue);
}

void MainWindow::on_combo_Levels_activated(int index)
{
    Generator.setLevels(index);
}

void MainWindow::on_combo_RoomH_activated(int index)
{
    Generator.setRoomHeight(index + 4);
}

void MainWindow::on_combo_RoomW_activated(int index)
{
    Generator.setRoomWidth(index + 4);
}

void MainWindow::on_combo_Boxes_activated(int index)
{
    Generator.setBoxes(index);
}

void MainWindow::on_combo_Difficulty_activated(int index)
{
    Generator.setDifficulty(index);
}

void MainWindow::on_generateButton_released()
{
    Generator.clearVectors();
    ui->list_LevelSet->clear();
    Generator.generateLevel();
}
