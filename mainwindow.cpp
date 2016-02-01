#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sokogenerator.h"
#include <QTCore>
#include <QtGui>
#include <QMessageBox>

SokoGenerator Generator;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_GenerationTime->setText("Estimated Generation Time: 0 Hours 0 Minutes");
    ui->progressBar->setValue(0);

    connect(&Generator, SIGNAL(changeProgressBar(float)), this, SLOT(changeProgressBar(float)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeProgressBar(float value){
    ui->progressBar->setValue(value);
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
    Generator.generateLevel();
}
