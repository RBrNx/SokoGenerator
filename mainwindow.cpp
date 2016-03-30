#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sokogenerator.h"
#include <iostream>

SokoGenerator Generator;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_GenerationTime->setText("Current Generation Time: 00:00:00");
    ui->progressBar->setValue(0);
    ui->progressBar->setAlignment(Qt::AlignRight);
    ui->list_LevelSet->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lineEdit_GeneratorSeed->setValidator(new QIntValidator(0,999999999,this));
    ui->lineEdit_GeneratorSeed->setText(0000000000);
    ui->spin_TimeLimit->setValue(1.00);

    connect(&Generator, SIGNAL(changeProgressBar(float)), this, SLOT(changeProgressBar(float)));
    connect(&Generator, SIGNAL(addToList(int)), this, SLOT(addToList(int)));
    connect(&Generator, SIGNAL(regenFinished(int)), this, SLOT(displayLevel(int)));
    connect(&Generator, SIGNAL(regenFinished(int)), this, SLOT(displayLevelGenTime(int)));
    connect(&thread, SIGNAL(finished()), this, SLOT(stopTimer()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(&Generator, SIGNAL(resetGUI()), this, SLOT(resetGUI()));
    connect(ui->list_LevelSet, SIGNAL(currentRowChanged(int)), this, SLOT(displayLevel(int)));
    connect(ui->combo_RoomH, SIGNAL(currentTextChanged(QString)), this, SLOT(disable3by3(QString)));
    connect(ui->combo_RoomW, SIGNAL(currentTextChanged(QString)), this, SLOT(disable3by3(QString)));
    connect(ui->list_LevelSet, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClickMenu(QPoint)));
    connect(&Generator, SIGNAL(displayGenSeed()), this, SLOT(displayGenSeed()));

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    Generator.setupForThread(thread);
    Generator.moveToThread(&thread);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
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

void MainWindow::updateTimer(){
    int diffTime = Time.elapsed();
    genTime = genTime.addMSecs(diffTime - lastTime);
    lastTime = diffTime;
    ui->label_GenerationTime->setText("Current Generation Time: " + genTime.toString("mm:ss:zzz"));
}

void MainWindow::stopTimer(){
    timer.stop();
    lastTime = 0;
}

void MainWindow::addToList(int value){
    vector<SokoGenerator::Level> levels = Generator.getLevels();
    int millis = levels[value-1].generationTime % 1000;
    int seconds = ((int)levels[value-1].generationTime / 1000) % 60 ;
    int minutes = ((int)levels[value-1].generationTime / (1000*60)) % 60;
    QString padMillis = QString("%1").arg(millis, 3, 10, QChar('0'));
    QString padSeconds = QString("%1").arg(seconds, 2, 10, QChar('0'));
    QString padMinutes = QString("%1").arg(minutes, 2, 10, QChar('0'));
    QString diff = levels[value-1].difficulty;
    ui->list_LevelSet->addItem("Level " + QString::number(value) + " - " + padMinutes + ":" + padSeconds + ":" + padMillis + " - " + diff);
    QVariant dataValue(value-1);
    ui->list_LevelSet->item(value-1)->setData(Qt::UserRole, dataValue);
}

void MainWindow::displayLevel(int value){
    if (display){
        scene->clear();
        displayLevelOnScreen(value);

        QRectF bounds = scene->itemsBoundingRect();
        ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
        ui->graphicsView->centerOn(0, 0);
    }
}

void MainWindow::displayLevelGenTime(int lvlNum){
    vector<SokoGenerator::Level> levels = Generator.getLevels();
    int millis = levels[lvlNum].generationTime % 1000;
    int seconds = ((int)levels[lvlNum].generationTime / 1000) % 60 ;
    int minutes = ((int)levels[lvlNum].generationTime / (1000*60)) % 60;
    QString padMillis = QString("%1").arg(millis, 3, 10, QChar('0'));
    QString padSeconds = QString("%1").arg(seconds, 2, 10, QChar('0'));
    QString padMinutes = QString("%1").arg(minutes, 2, 10, QChar('0'));
    QString diff = levels[lvlNum].difficulty;
    ui->list_LevelSet->item(lvlNum)->setText("Level " + QString::number(lvlNum+1) + " - " + padMinutes + ":" + padSeconds + ":" + padMillis + " - " + diff);
}

void MainWindow::displayLevelOnScreen(int levelNum){
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
                sprite2 = new QGraphicsPixmapItem(QPixmap(":/tileset/textures/box_shipped.png"));
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
    Generator.setBoxes(index+2);
}

void MainWindow::on_combo_Difficulty_currentIndexChanged(int index)
{
    Generator.setDifficulty(index);
}

void MainWindow::on_generateButton_released()
{
    display = false;
    Generator.solver.setThreadStop(false);
    timer.stop();
    genTime = QTime::fromString("00:00:00:001", "hh:mm:ss:zzz");
    Generator.clearVectors();
    ui->list_LevelSet->clear();
    scene->clear();
    ui->progressBar->setValue(0);
    if(ui->lineEdit_GeneratorSeed->text() != ""){
        Generator.setGenerateSeed(genSeed);
    } else {
        Generator.setGenerateSeed(0);
    }
    display = true;
    timer.start();
    Time.start();
    thread.start();
}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionNew_triggered()
{
    Generator.solver.setThreadStop(true);
    if(thread.isFinished()){
        resetGUI();
    }
}

void MainWindow::resetGUI(){
    display = false;
    scene->clear();
    ui->list_LevelSet->clear();
    ui->progressBar->setValue(0);
    ui->combo_Boxes->setCurrentIndex(0);
    ui->combo_Difficulty->setCurrentIndex(0);
    ui->combo_Levels->setCurrentIndex(0);
    ui->combo_RoomH->setCurrentIndex(0);
    ui->combo_RoomW->setCurrentIndex(0);
    ui->spin_TimeLimit->setValue(1.00);
    ui->lineEdit_GeneratorSeed->setText(0000000000);
    timer.stop();
    ui->label_GenerationTime->setText("Current Generation Time: 00:00:00");
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
            stream << levelSet[lvlNum].solution.c_str();
            stream << "\n\n\n";
        }

        myFile.close();
    }
}

void MainWindow::rightClickMenu(const QPoint &pos){
    QPoint PItem = ui->list_LevelSet->mapToGlobal(pos);
    if(ui->list_LevelSet->indexAt(pos).row() >= 0){
        QMenu submenu;
        submenu.addAction("Regenerate Level");
        submenu.addAction("Delete Level");
        submenu.addAction("View Solution");
        QAction* rightClickItem = submenu.exec(PItem);

        if(rightClickItem && rightClickItem->text().contains("Delete Level")){
            QListWidgetItem* item = ui->list_LevelSet->takeItem(ui->list_LevelSet->indexAt(pos).row());
            if(item != NULL){
                int levelNum = item->data(Qt::UserRole).toInt();
                Generator.deleteLevel(levelNum);
                vector<SokoGenerator::Level> levelSet = Generator.getLevels();
                for(int i = levelNum; i < ui->list_LevelSet->count(); i++){
                    ui->list_LevelSet->item(i)->setData(Qt::UserRole, i);

                    int millis = levelSet[i].generationTime % 1000;
                    int seconds = ((int)levelSet[i].generationTime / 1000) % 60 ;
                    int minutes = ((int)levelSet[i].generationTime / (1000*60)) % 60;
                    QString padMillis = QString("%1").arg(millis, 3, 10, QChar('0'));
                    QString padSeconds = QString("%1").arg(seconds, 2, 10, QChar('0'));
                    QString padMinutes = QString("%1").arg(minutes, 2, 10, QChar('0'));
                    QString diff = levelSet[i].difficulty;
                    ui->list_LevelSet->item(i)->setText("Level " + QString::number(i+1) + " - " + padMinutes + ":" + padSeconds + ":" + padMillis + " - " + diff);
                }
                if(item->data(Qt::UserRole).toInt() == ui->list_LevelSet->count()){
                    ui->list_LevelSet->setCurrentRow(ui->list_LevelSet->count()-1);
                }
                else{
                    ui->list_LevelSet->setCurrentRow(item->data(Qt::UserRole).toInt());
                }

                delete item;
            }
        }
        else if(rightClickItem && rightClickItem->text().contains("Regenerate Level")){
            int row = ui->list_LevelSet->indexAt(pos).row();
            if(row >= 0){
                regenerateLevel(row);
                displayLevel(row);
            }
        }
        else if(rightClickItem && rightClickItem->text().contains("View Solution")){
            int row = ui->list_LevelSet->indexAt(pos).row();
            if(row >= 0){
                QString solution = createSolution(row);
                QMessageBox::about(this, "Level " + QString::number(row+1) + " Solution", solution);
            }
        }

    }

}

QString MainWindow::createSolution(int lvlNum){
    vector<SokoGenerator::Level> levelSet = Generator.getLevels();
    QString solution = QString::fromStdString(levelSet[lvlNum].solution);
    for(int i = 0; i < solution.length(); i++){
        if(i % 50 == 0){
            solution.insert(i, QChar('\n'));
        }
    }

    solution+= "\n\n";
    solution += "--------------------";
    solution += "\n\n";

    QString RLESolution = QString::fromStdString(levelSet[lvlNum].solution);
    int moveCount = 0;
    for(int i = 0; i < RLESolution.length(); i++){
        int charCount = 1;
        QChar C = RLESolution[i];

        while(C == RLESolution[i+1]) {
            charCount++;
            i++;
        }

        if(charCount > 1){
            solution += C;
            solution += QString::number(charCount);
            solution += "  ";
            moveCount++;
        }
        else{
            solution += C;
            solution += "  ";
            moveCount++;
        }

        if(moveCount % 20 == 0){
            solution += "\n";
        }

    }

    return solution;
}

void MainWindow::regenerateLevel(int lvlNum){
    Generator.setRegenerate(true, lvlNum);
    thread.start();
    timer.start();
    Time.start();
}

void MainWindow::on_spin_TimeLimit_valueChanged(double timeLimit)
{
    Generator.setTimeout(timeLimit);
}

void MainWindow::displayGenSeed(){
    ui->lineEdit_GeneratorSeed->setText(QString::number(Generator.getGenSeed()));
    qDebug() << "Gen Seed";
}

void MainWindow::on_lineEdit_GeneratorSeed_textEdited(const QString &arg1)
{
    genSeed = arg1.toInt();
}

void MainWindow::on_actionAbout_SokoGenerator_triggered()
{
    QMessageBox::about(this, "About SokoGenrator", "<img src=':/tileset/textures/logo.png' align = 'left'></img>"
                                                   "<p align = 'center'>"
                                                   "<h2>Version 1.0.1</h2>"
                                                   "<h4>Developed by <a href='https://github.com/RBrNx'> Conor Watson </a></h4>"
                                                   "<h4>Special Thanks to <a href='http://pavel.klavik.cz/'> Pavel Klavik </a></h4>"
                                                   "<h4>Developed using C++ and QT</h4>"
                                                   "<h4>SokoGenerator Licensed under MIT 2016</h4>"
                                                   "<h4><a href='https://github.com/RBrNx/SokoGenerator#help'>Needing Help?</a></h4>"
                                                   "</p>");
}
