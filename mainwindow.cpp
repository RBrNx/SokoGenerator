#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTCore>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinner_RoomH->setRange(1,20);
    ui->spinner_RoomW->setRange(1,20);
    ui->spinner_Boxes->setRange(1,4);
    ui->spinner_Levels->setRange(1,20);

    ui->label_GenerationTime->setText("Estimated Generation Time: 0 Hours 0 Minutes");
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
