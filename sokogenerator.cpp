#include "sokogenerator.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <time.h>

SokoGenerator::SokoGenerator(QObject *parent):QObject(parent){
    roomHeight = 0;
    roomWidth = 0;
    noOfBoxes = 0;
    noOfLevels = 0;
    difficulty = 0;

    percentage = 0;
}

SokoGenerator::~SokoGenerator(){

}

int SokoGenerator::randomNumber(int min, int max){
    return rand() % (max-min)+min;
}

void SokoGenerator::generateLevel(){
    srand(time(NULL));
    int _levels;

    if(noOfLevels == 0){ _levels = randomNumber(1, 20); } else { _levels = noOfLevels; }

    for(int i = 0; i < _levels; i++){
        generateLevel(roomWidth, roomHeight, noOfBoxes, difficulty, i);
        float perc = ((i + 1) * 100) / _levels;
        updatePercentage(perc);
    }


}

void SokoGenerator::generateLevel(int roomWidth, int roomHeight, int noOfBoxes, int difficulty, int levelNumber){
    int _roomW, _roomH, _Boxes, _difficulty;
    srand(time(NULL));

    if(roomWidth == 0){ _roomW = randomNumber(5, 20); } else { _roomW = roomWidth; }
    if(roomHeight == 0){ _roomH = randomNumber(5, 20); } else { _roomH = roomHeight; }
    if(noOfBoxes == 0){ _Boxes = randomNumber(1, 4); } else { _Boxes = noOfBoxes; }
    if(difficulty == 0){ _difficulty = randomNumber(1, 5); } else { _difficulty = difficulty; }

    QMessageBox::information(NULL, "Information", "Generated Level: " + QString::number(levelNumber) + "\n Room Width: " + QString::number(_roomW) + "\n Room Height: "
                             + QString::number(_roomH) + "\n No of Boxes: " + QString::number(_Boxes) + "\n Difficulty: " + QString::number(_difficulty));
}

void SokoGenerator::updatePercentage(float value){
    emit changeProgressBar(value);
}
