#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sokogenerator.h"
#include <iostream>

SokoGenerator Generator;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_GenerationTime->setText("Estimated Generation Time: 0 Hours 0 Minutes");
    ui->progressBar->setValue(0);

    connect(&Generator, SIGNAL(changeProgressBar(float)), this, SLOT(changeProgressBar(float)));
    connect(&Generator, SIGNAL(addToList(int)), this, SLOT(addToList(int)));
    connect(ui->list_LevelSet, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(displayLevel(QListWidgetItem*)));
    connect(ui->combo_RoomH, SIGNAL(currentTextChanged(QString)), this, SLOT(disable3by3(QString)));
    connect(ui->combo_RoomW, SIGNAL(currentTextChanged(QString)), this, SLOT(disable3by3(QString)));

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disable3by3(QString value){
    if(ui->combo_RoomW->currentText() == "3" && roomHRemoved == false){
        qobject_cast<QStandardItemModel *> (ui->combo_RoomH->model())->item(1)->setEnabled(false);
        roomHRemoved = true;
    }
    else if (ui->combo_RoomH->currentText() == "3" && roomWRemoved == false){
        qobject_cast<QStandardItemModel *> (ui->combo_RoomW->model())->item(1)->setEnabled(false);
        roomWRemoved = true;
    }
    else{
        if(roomWRemoved){
           roomWRemoved = false;
           qobject_cast<QStandardItemModel *> (ui->combo_RoomW->model())->item(1)->setEnabled(true);
        }
        if(roomHRemoved){
           qobject_cast<QStandardItemModel *> (ui->combo_RoomH->model())->item(1)->setEnabled(true);
           roomHRemoved = false;
        }
    }
}

void MainWindow::changeProgressBar(float value){
    ui->progressBar->setValue(value);
}

void MainWindow::addToList(int value){
    ui->list_LevelSet->addItem("Level " + QString::number(value));
    QVariant dataValue(value-1);
    ui->list_LevelSet->item(value-1)->setData(Qt::UserRole, dataValue);
}

void MainWindow::displayLevel(QListWidgetItem* item){
    //int value = QVariant.toInt(item->data(Qt::UserRole));
    scene->clear();
    int value = item->data(Qt::UserRole).toInt();
    displayLevel(value);

    QRectF bounds = scene->itemsBoundingRect();
    ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(0, 0);
}

void MainWindow::displayLevel(int levelNum){
    std::vector< std::vector<char> > level = Generator.getLevel(levelNum);

    for(int y = 0; y < level.size(); y++){
        for(int x = 0; x < level[y].size(); x++){

            if(level[y].at(x) == '#'){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/wall.png"));
                sprite2 = NULL;
            }
            else if(level[y].at(x) == ' '){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/floor.png"));
                sprite2 = NULL;
            }
            else if(level[y].at(x) == '@'){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/floor.png"));
                sprite2 = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/robot1_hold.png"));
            }
            else if(level[y].at(x) == '+'){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/goal.png"));
                sprite2 = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/robot1_hold.png"));
            }
            else if(level[y].at(x) == '$'){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/floor.png"));
                sprite2 = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/box.png"));
            }
            else if(level[y].at(x) == '*'){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/goal.png"));
                sprite2 = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/floor.png"));
            }
            else if(level[y].at(x) == '.'){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/floor.png"));;
                sprite2 = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/goal.png"));
            }

            sprite->setPos(x * 64, y * 64);
            scene->addItem(sprite);
            if(sprite2 != NULL){
                sprite2->setPos(x * 64, y * 64);
                scene->addItem(sprite2);
            }
        }
    }

}

void MainWindow::on_combo_Levels_activated(int index)
{
    Generator.setLevels(index);
}

void MainWindow::on_combo_RoomH_activated(int index)
{
    Generator.setRoomHeight(index * 3);
}

void MainWindow::on_combo_RoomW_activated(int index)
{
    Generator.setRoomWidth(index * 3);
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
