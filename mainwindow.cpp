#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sokogenerator.h"
#include <QTCore>
#include <QtGui>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_GenerationTime->setText("Estimated Generation Time: 0 Hours 0 Minutes");
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_combo_Levels_activated(int index)
{
    SokoGenerator::setLevels(index);
}

void MainWindow::on_combo_RoomH_activated(int index)
{
    SokoGenerator::setRoomHeight(index);
}

void MainWindow::on_combo_RoomW_activated(int index)
{
    SokoGenerator::setRoomWidth(index);
}

void MainWindow::on_combo_Boxes_activated(int index)
{
    SokoGenerator::setBoxes(index);
}

void MainWindow::on_combo_Difficulty_activated(int index)
{
    SokoGenerator::setDifficulty(index);
}

void MainWindow::on_generateButton_released()
{
    SokoGenerator::generateLevel();
}
