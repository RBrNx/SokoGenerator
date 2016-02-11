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
    ui->list_LevelSet->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(&Generator, SIGNAL(changeProgressBar(float)), this, SLOT(changeProgressBar(float)));
    connect(&Generator, SIGNAL(addToList(int)), this, SLOT(addToList(int)));
    connect(ui->list_LevelSet, SIGNAL(currentRowChanged(int)), this, SLOT(displayLevel(int)));
    //connect(ui->list_LevelSet, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(displayLevel(QListWidgetItem*)));
    connect(ui->combo_RoomH, SIGNAL(currentTextChanged(QString)), this, SLOT(disable3by3(QString)));
    connect(ui->combo_RoomW, SIGNAL(currentTextChanged(QString)), this, SLOT(disable3by3(QString)));
    connect(ui->list_LevelSet, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClickMenu(QPoint)));

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disable3by3(QString /*unused*/){
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

void MainWindow::displayLevel(int value){
//void MainWindow::displayLevel(QListWidgetItem* item){
    if (display){
        scene->clear();
        //int value = item->data(Qt::UserRole).toInt();
        displayLevelOnScreen(value);
        //displayLevel(value);

        QRectF bounds = scene->itemsBoundingRect();
        ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
        ui->graphicsView->centerOn(0, 0);
    }
}

void MainWindow::displayLevelOnScreen(int levelNum){
//void MainWindow::displayLevel(int levelNum){
    std::vector< std::vector<char> > level = Generator.getLevel(levelNum);

    for(size_t y = 0; y < level.size(); y++){
        for(size_t x = 0; x < level[y].size(); x++){

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
                sprite2 = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/box.png"));
            }
            else if(level[y].at(x) == '.'){
                sprite = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/goal.png"));;
                sprite2 = NULL;
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

void MainWindow::on_combo_Levels_currentIndexChanged(int index)
{
    Generator.setLevels(index);
}

void MainWindow::on_combo_RoomH_currentIndexChanged(int index)
{
    Generator.setRoomHeight(index * 3);
}

void MainWindow::on_combo_RoomW_currentIndexChanged(int index)
{
    Generator.setRoomWidth(index * 3);
}

void MainWindow::on_combo_Boxes_currentIndexChanged(int index)
{
    Generator.setBoxes(index);
}

void MainWindow::on_combo_Difficulty_currentIndexChanged(int index)
{
    Generator.setDifficulty(index);
}

void MainWindow::on_generateButton_released()
{
    display = false;
    Generator.clearVectors();
    ui->list_LevelSet->clear();
    display = true;
    Generator.generateLevel();
}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionNew_triggered()
{
    display = false;
    scene->clear();
    ui->list_LevelSet->clear();
    ui->progressBar->setValue(0);
    ui->combo_Boxes->setCurrentIndex(0);
    ui->combo_Difficulty->setCurrentIndex(0);
    ui->combo_Levels->setCurrentIndex(0);
    ui->combo_RoomH->setCurrentIndex(0);
    ui->combo_RoomW->setCurrentIndex(0);
    display = true;
}

void MainWindow::on_actionSave_As_triggered()
{
    vector<SokoGenerator::Level> levelSet = Generator.getLevels();
    QString docLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), docLocation, "Sokoban Level Sets (*.sok)");

    QFile myFile(filename);
    if(myFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream stream(&myFile);
        QStringList filepath = filename.split("/").last().split(".");

        stream << "Author: " << qgetenv("USERNAME") << "\n";
        stream << "Title: " << filepath.first() << "\n\n\n";

        for(size_t lvlNum = 0; lvlNum < levelSet.size(); lvlNum++){

            stream << filepath.first() << " " << lvlNum + 1 << "\n\n";
            for(size_t column = 0; column < levelSet[lvlNum].grid.size(); column++){
                for(size_t row = 0; row < levelSet[lvlNum].grid[column].size(); row++){
                    stream << levelSet[lvlNum].grid[column][row];
                }
                stream << "\n";
            }
            stream << "\n\n\n";
            stream << "Solution/Moves\n";
            stream << "\n\n\n";
        }

        myFile.close();
    }
}

void MainWindow::rightClickMenu(const QPoint &pos){
    QPoint PItem = ui->list_LevelSet->mapToGlobal(pos);
    QMenu submenu;
    submenu.addAction("Regenerate Level");
    submenu.addAction("Delete Level");
    QAction* rightClickItem = submenu.exec(PItem);
    if(rightClickItem && rightClickItem->text().contains("Delete Level")){
        QListWidgetItem* item = ui->list_LevelSet->takeItem(ui->list_LevelSet->indexAt(pos).row());
        Generator.deleteLevel(item->data(Qt::UserRole).toInt()+1);
        for(int i = 0; i < ui->list_LevelSet->count(); i++){
            ui->list_LevelSet->item(i)->setData(Qt::UserRole, i);
            ui->list_LevelSet->item(i)->setText("Level " + QString::number(i+1));
        }
        ui->list_LevelSet->setCurrentRow(item->data(Qt::UserRole).toInt());
        delete item;
    }
    else if(rightClickItem && rightClickItem->text().contains("Regenerate Level")){
        regenerateLevel(ui->list_LevelSet->indexAt(pos).row());
        ui->list_LevelSet->setCurrentRow(ui->list_LevelSet->indexAt(pos).row() + 1);
        ui->list_LevelSet->setCurrentRow(ui->list_LevelSet->indexAt(pos).row());
    }

}

void MainWindow::regenerateLevel(int lvlNum){
    Generator.regenerateLevel(lvlNum);
}
