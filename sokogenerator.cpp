#include "sokogenerator.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <time.h>
#include <iostream>

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

    listLevelSet(levels);
}

void SokoGenerator::generateLevel(int roomWidth, int roomHeight, int noOfBoxes, int difficulty, int levelNumber){
    int _roomW, _roomH, _Boxes, _difficulty;

    if(roomWidth == 0){ _roomW = randomNumber(5, 12); } else { _roomW = roomWidth; }
    if(roomHeight == 0){ _roomH = randomNumber(5, 12); } else { _roomH = roomHeight; }
    if(noOfBoxes == 0){ _Boxes = randomNumber(1, 3); } else { _Boxes = noOfBoxes; }
    if(difficulty == 0){ _difficulty = randomNumber(1, 5); } else { _difficulty = difficulty; }

    Level newLevel;

    initLevel(&newLevel, _roomW, _roomH);

    levels.push_back(newLevel);
}

void SokoGenerator::initLevel(SokoGenerator::Level *level, int roomWidth, int roomHeight){
    std::vector<char> row;

    for(int y = 0; y < roomHeight; y++){

        for(int x = 0; x < roomWidth; x++){

            if(y == 0 || y == roomHeight-1 || x == 0 || x == roomWidth-1){
                row.push_back(WALL);
            }
            else{
                row.push_back(FLOOR);
            }

        }

        level->grid.push_back(row);
        row.clear();
    }

    level->grid[randomNumber(1, roomHeight-1)].at(randomNumber(1, roomWidth-1)) = '@';
}

std::vector< std::vector<char> > SokoGenerator::getLevel(int level){

    return levels[level].grid;
}

void SokoGenerator::clearVectors(){
    levels.clear();
}

void SokoGenerator::updatePercentage(float value){
    emit changeProgressBar(value);
}

void SokoGenerator::listLevelSet(std::vector<Level> levels){
    for(int i = 0; i < levels.size(); i++){
         emit addToList(i + 1);
    }
}
