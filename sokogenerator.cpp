#include "sokogenerator.h"
#include "mainwindow.h"
#include <QMessageBox>

SokoGenerator::SokoGenerator(QObject *parent):QObject(parent){
    roomHeight = 0;
    roomWidth = 0;
    noOfBoxes = 0;
    noOfLevels = 0;
    difficulty = 0;

    percentage = 0;

    for(int i = 1; i < 18; i++){
        QString filename = ":/patterns/patterns/" + QString::number(i) + ".txt";
        QString line;
        std::vector<char> row;
        QFile myFile(filename);
        Level pattern;

        if (myFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&myFile);
            while (!in.atEnd())
            {
                line = in.readLine();
                for(int j = 0; j < line.size(); j++){
                    row.push_back(line.at(j).toLatin1());
                }
                pattern.grid.push_back(row);
                row.clear();
            }
            myFile.close();
        }
        else
        {
            cout << "Unable to open file";
        }

        patterns.push_back(pattern);
    }
}

SokoGenerator::~SokoGenerator(){

}

int SokoGenerator::randomNumber(int min, int max, int divisor){
    int number = rand() % (max-min)+min;

    while(number % divisor != 0){
        number = rand() % (max-min)+min;
    }
    return number;
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
    if(roomWidth == 0){ _roomW = randomNumber(3, 12, 3); } else { _roomW = roomWidth; }
    if(roomHeight == 0){ (_roomW == 3) ? _roomH = randomNumber(6, 12, 3) : _roomH = randomNumber(3, 12, 3); }
    if(noOfBoxes == 0){ _Boxes = randomNumber(1, 3); } else { _Boxes = noOfBoxes; }
    if(difficulty == 0){ _difficulty = randomNumber(1, 5); } else { _difficulty = difficulty; }

    Level newLevel;

    initLevel(&newLevel, _roomW, _roomH);
    placePatterns(&newLevel, _roomW, _roomH);

    levels.push_back(newLevel);
    cout << "Level Generated";
}

void SokoGenerator::initLevel(SokoGenerator::Level *level, int roomWidth, int roomHeight){
    std::vector<char> row;
    roomHeight = roomHeight + 2;
    roomWidth = roomWidth + 2;

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

    //level->grid[randomNumber(1, roomHeight-1)].at(randomNumber(1, roomWidth-1)) = '@';
}

void SokoGenerator::placePatterns(SokoGenerator::Level *level, int roomWidth, int roomHeight){
    int patternPlacedCount = 0;
    Level tempLevel;

    for(int y = 1; y < roomHeight; y++){

        for(int x = 1; x < roomWidth; x++){

            if((y - 1) % 3 == 0 && (x - 1) % 3 == 0){
                while(patternPlacedCount != 25){
                    tempLevel = *level;
                    patternPlacedCount = 0;
                    twoDVector chosenPattern = patterns[randomNumber(0, 17)].grid;
                    rotatePattern(&chosenPattern, randomNumber(0, 3));
                    for(int pY = 0; pY < chosenPattern.size(); pY++){
                        for(int pX = 0; pX < chosenPattern[pY].size(); pX++){

                            if(pX == 0 || pX == chosenPattern[pY].size()-1 || pY == 0 || pY == chosenPattern.size()-1){
                                if(chosenPattern[pY][pX] == ' '){
                                    if(tempLevel.grid[y + pY -1][x + pX - 1] != ' '){
                                        patternPlacedCount = -100;
                                    }
                                    else{
                                        patternPlacedCount++;
                                    }
                                }
                                else{
                                    patternPlacedCount++;
                                }
                            }
                            else if(chosenPattern[pY][pX] != '-'){
                               tempLevel.grid[y + pY - 1][x + pX - 1] = chosenPattern[pY][pX];
                               patternPlacedCount++;
                            }
                            else{
                                patternPlacedCount++;
                            }
                        }
                    }
                }
                *level = tempLevel;
                patternPlacedCount = 0;
            }

        }
    }
}

void SokoGenerator::rotatePattern(twoDVector *pattern, int rotation){
    twoDVector tempPattern = *pattern;
    if(rotation == 1){
        //Rotate by 90 - reverse each row
        for(int i = 0; i < tempPattern.size(); i++){
            std::reverse(tempPattern[i].begin(), tempPattern[i].end());
        }
        *pattern = tempPattern;
    }
    else if(rotation == 2){
        //Rotate by 180 - reverse each row, then each column
        for(int i = 0; i < tempPattern.size(); i++){
            std::reverse(tempPattern[i].begin(), tempPattern[i].end());
        }

        std::reverse(tempPattern.begin(), tempPattern.end());
        *pattern = tempPattern;
    }
    else if(rotation == 3){
        //Rotate by 270 - reverse each column
        std::reverse(tempPattern.begin(), tempPattern.end());
        *pattern = tempPattern;
    }
}

SokoGenerator::twoDVector SokoGenerator::getLevel(int level){
    return levels[level].grid ;
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
