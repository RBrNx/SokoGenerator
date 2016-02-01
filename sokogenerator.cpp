#include "sokogenerator.h"
#include <QMessageBox>



int SokoGenerator::roomWidth = 0;
int SokoGenerator::roomHeight = 0;
int SokoGenerator::noOfBoxes =0;
int SokoGenerator::noOfLevels =0;
int SokoGenerator::difficulty = 0;
int SokoGenerator::percentage =0;



SokoGenerator::SokoGenerator(){

}

SokoGenerator::~SokoGenerator(){

}

void SokoGenerator::generateLevel(){
    generateLevel(noOfLevels);
}

void SokoGenerator::generateLevel(int noOfLevels){
    for(int i = 0; i < noOfLevels; i++){
        QMessageBox::information(NULL, "Information", "No of Levels: " + QString::number(noOfLevels) + "\n Room Width: " + QString::number(roomWidth) + "\n Room Height: " + QString::number(roomHeight) + "\n No of Boxes: " + QString::number(noOfBoxes) + "\n Difficulty: " + QString::number(difficulty));
    }
}
