#include "sokogenerator.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "difficultyanalyser.h"


SokoGenerator::SokoGenerator(QObject *parent):QObject(parent){
    initialSetup();
}

SokoGenerator::~SokoGenerator(){

}

void SokoGenerator::setupForThread(QThread &thread){
    connect(&thread, SIGNAL(started()), this, SLOT(startThreadWork()));
    connect(this, SIGNAL(threadFinished()), &thread, SLOT(quit()));
    connect(this, SIGNAL(regenFinished(int)), &thread, SLOT(quit()));
}

void SokoGenerator::startThreadWork(){
    if(regenLevel){
        regenerateLevel(regenLvlNum);
        regenLevel = false;
    }
    else{
        generateLevel();
    }
}

void SokoGenerator::initialSetup(){
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

SokoGenerator::ull SokoGenerator::randomNumber(ull min, ull max, int divisor){
    ull number = rand() % (max - min + 1)+ min;

    while(number % divisor != 0){
        number = rand() % (max - min + 1) + min;
    }

    return number;
}

void SokoGenerator::generateLevel(){
    generator.seed(chrono::steady_clock::now().time_since_epoch().count());
    if(genSeed == 0){
        while(genSeed > 999999999 || genSeed == 0){
            genSeed = distribution(generator);
        }
    }
    srand(genSeed);
    emit displayGenSeed();
    int _levels;

    if(noOfLevels == 0){ _levels = randomNumber(1, 20); } else { _levels = noOfLevels; }

    for(int i = 0; i < _levels; i++){
        generateLevel(roomWidth, roomHeight, noOfBoxes, difficulty);
        float perc = ((i + 1) * 100) / _levels;
        updatePercentage(perc);
    }

    listLevelSet(levels);
    emit threadFinished();
    if(threadStop) { emit resetGUI(); }
}

void SokoGenerator::generateLevel(int roomWidth, int roomHeight, int noOfBoxes, int difficulty){
    bool generationSuccessful = false;
    Level newLevel;
    QTime timer;
    timer.start();
    clock_t start = clock();

    while(!generationSuccessful && !threadStop){
        if(isTimeout(start, 1.0f)){
            generationSuccessful = false;
        }

        newLevel.grid.clear();
        int _roomW, _roomH, _Boxes;
        QString _difficulty;
        if(noOfBoxes == 0){ _Boxes = randomNumber(3, 6); } else { _Boxes = noOfBoxes; }
        if(difficulty == 0) { _difficulty = difficulties[randomNumber(1, 5)]; } else { _difficulty = difficulties[difficulty]; }
        if(roomWidth == 0){ _roomW = randomNumber(3, 15, 3); } else { _roomW = roomWidth; }
        if(roomHeight == 0){
            if(_roomW == 3){ _roomH = randomNumber(6, 15, 3); }
            else { _roomH = randomNumber(3, 15, 3); }
        }
        else {
            _roomH = roomHeight;
        }

        initLevel(newLevel, _roomW, _roomH);
        placePatterns(newLevel, _roomW, _roomH);
        generationSuccessful = checkConnectivity(newLevel, _roomW, _roomH, _Boxes);
        if(generationSuccessful) generationSuccessful = placeGoalsAndBoxes(newLevel, _roomW, _roomH, _Boxes);
        if(generationSuccessful) generationSuccessful = placePlayer(newLevel, _roomW, _roomH);
        if(generationSuccessful){
            level lvl = LevelToCLevel(newLevel);
            struct solution sol;
            qDebug() << "Generation Started: ";
            generationSuccessful = solver.solve(lvl, timeout, sol);
            if(generationSuccessful){
                newLevel.solution = cSolToString(sol);
                diffAnalyser = new DifficultyAnalyser;
                newLevel.difficulty = diffAnalyser->calculateDifficulty(newLevel);
                if(difficulty > 0){
                    if(newLevel.difficulty != _difficulty) generationSuccessful = false;
                }
                delete diffAnalyser;
            }
            qDebug() << "Generation Successful: " << generationSuccessful;
        }
    }
    newLevel.generationTime = timer.elapsed();
    levels.push_back(newLevel);
}

void SokoGenerator::initLevel(SokoGenerator::Level &level, int roomWidth, int roomHeight){
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

        level.grid.push_back(row);
        row.clear();
    }
}

void SokoGenerator::placePatterns(SokoGenerator::Level &level, int roomWidth, int roomHeight){
    int patternPlacedCount = 0;
    Level tempLevel;

    for(int y = 1; y < roomHeight; y++){

        for(int x = 1; x < roomWidth; x++){

            if((y - 1) % 3 == 0 && (x - 1) % 3 == 0){
                while(patternPlacedCount != 25){
                    tempLevel = level;
                    patternPlacedCount = 0;
                    TwoDVector_char chosenPattern = patterns[randomNumber(0, 16)].grid;
                    rotatePattern(chosenPattern, randomNumber(0, 3));
                    for(size_t pY = 0; pY < chosenPattern.size(); pY++){
                        for(size_t pX = 0; pX < chosenPattern[pY].size(); pX++){

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
                level = tempLevel;
                patternPlacedCount = 0;
            }

        }
    }
}

void SokoGenerator::rotatePattern(TwoDVector_char &pattern, int rotation){
    TwoDVector_char tempPattern = pattern;
    if(rotation == 1){
        //Rotate by 90 - reverse each row
        for(size_t i = 0; i < tempPattern.size(); i++){
            std::reverse(tempPattern[i].begin(), tempPattern[i].end());
        }
    }
    else if(rotation == 2){
        //Rotate by 180 - reverse each row, then each column
        for(size_t i = 0; i < tempPattern.size(); i++){
            std::reverse(tempPattern[i].begin(), tempPattern[i].end());
        }

        std::reverse(tempPattern.begin(), tempPattern.end());
    }
    else if(rotation == 3){
        //Rotate by 270 - reverse each column
        std::reverse(tempPattern.begin(), tempPattern.end());
    }

    pattern = tempPattern;
}

bool SokoGenerator::checkConnectivity(SokoGenerator::Level &level, int roomWidth, int roomHeight, int noOfBoxes){
    TwoDVector_int tempLevel;
    bool floorFound = false;
    vector<int> tempRow;
    size_t wallCount = 0;
    int floorCount = 0;

    for(int column = 0; column < roomHeight+2; column++){
        for(int row = 0; row < roomWidth+2; row++){
            if(level.grid[column][row] == FLOOR){
                tempRow.push_back(0);
            }
            else{
                tempRow.push_back(1);
            }
        }
        tempLevel.push_back(tempRow);
        tempRow.clear();
    }

    for(int column = 0; column < roomHeight+2; column++){
        for(int row = 0; row < roomWidth+2; row++){
            if(tempLevel[column][row] == 0 && floorFound == false){
                floorFound = true;
                floodfill(tempLevel, row, column, roomWidth+2, roomHeight+2);
            }
        }
    }

    for(int column = 0; column < roomHeight+2; column++){
        for(int row = 0; row < roomWidth+2; row++){
            if(tempLevel[column][row] == 0){
                return false;
            }
            else if (tempLevel[column][row] == 1) {
                wallCount++;
            }
            else if (tempLevel[column][row] == 2){
                floorCount++;
            }


            if (wallCount == tempLevel.size() * tempLevel[column].size()) {
                return false;
            }
        }
    }
    if(floorCount > noOfBoxes + noOfBoxes + 1 + 5){ //noOfBoxes + noOfGoals + player + extra floor tiles
        return true;
    }
    else{
        return false;
    }
}

void SokoGenerator::floodfill(TwoDVector_int &level, int row, int column, int roomWidth, int roomHeight){
    struct Coordinate {
        int column;
        int row;
    } coordinate;

    coordinate.column = column;
    coordinate.row = row;

    queue<Coordinate> levelQueue;

    levelQueue.push(coordinate);

    while(!levelQueue.empty()){
        Coordinate node = levelQueue.front();
        levelQueue.pop();
        if(level[node.column][node.row] == 0){
            level[node.column][node.row] = 2;
            if(node.row > 0) if(level[node.column][node.row-1] == 0){ levelQueue.push({node.column, node.row-1}); }
            if(node.row < roomWidth) if(level[node.column][node.row+1] == 0){ levelQueue.push({node.column, node.row+1}); }
            if(node.column > 0) if(level[node.column-1][node.row] == 0){ levelQueue.push({node.column-1, node.row}); }
            if(node.column < roomHeight) if(level[node.column+1][node.row] == 0){ levelQueue.push({node.column+1, node.row}); }
        }
    }
}

bool SokoGenerator::placeGoalsAndBoxes(SokoGenerator::Level &level, int roomWidth, int roomHeight, int noOfBoxes){
    bool goalsPlaced = false, boxesPlaced = false;
    int goalCount = 0, boxCount = 0;
    int xCoord = 0, yCoord = 0;
    int m = 0;
    clock_t start = clock();
    Level deadFields;

    while(!goalsPlaced && !threadStop){
        if(isTimeout(start, 1.0f)){
            break;
        }

        xCoord = randomNumber(1, roomWidth);
        yCoord = randomNumber(1, roomHeight);
        if(level.grid[yCoord][xCoord] == FLOOR){
            level.grid[yCoord][xCoord] = GOAL;
            goalCount++;
        }
        if(goalCount == noOfBoxes){
            goalsPlaced = true;
        }
    }

    deadFields = calcDeadFields(level);

    while(!boxesPlaced && !threadStop){
        if(isTimeout(start, 1.0f)){
            break;
        }
        xCoord = randomNumber(1, roomWidth);
        yCoord = randomNumber(1, roomHeight);
        if(deadFields.grid[yCoord][xCoord] == FLOOR){
            level.grid[yCoord][xCoord] = BOX;
            deadFields.grid[yCoord][xCoord] = BOX;
            boxCount++;
        }
        else if(level.grid[yCoord][xCoord] == GOAL){
            if(neighbourCheck(level, yCoord, xCoord) < 2 && boxCount < noOfBoxes-1){
                level.grid[yCoord][xCoord] = BOXONGOAL;
                boxCount++;
            }
        }
        if(boxCount == noOfBoxes){
            boxesPlaced = true;
        }
    }

    if(boxesPlaced && goalsPlaced){
        return true;
    }
    else{
        return false;
    }
}
bool SokoGenerator::placePlayer(SokoGenerator::Level &level, int roomWidth, int roomHeight){
    bool playerPlaced = false;
    int xCoord, yCoord;
    clock_t start = clock();

    while(!playerPlaced && !threadStop){
        if(isTimeout(start, 1.0f)){
            break;
        }

        xCoord = randomNumber(1, roomWidth);
        yCoord = randomNumber(1, roomHeight);
        if(level.grid[yCoord][xCoord] == FLOOR){
            level.grid[yCoord][xCoord] = PLAYER;
            playerPlaced = true;
        }
        else if(level.grid[yCoord][xCoord] == GOAL){
            level.grid[yCoord][xCoord] = PONGOAL;
            playerPlaced = true;
        }
    }

    return playerPlaced;
}

int SokoGenerator::neighbourCheck(SokoGenerator::Level &level, int yCoord, int xCoord){
    int neighbourWalls = 0;

    if(level.grid[yCoord-1][xCoord] == WALL){
        neighbourWalls++;
    }
    if(level.grid[yCoord+1][xCoord] == WALL){
        neighbourWalls++;
    }
    if(level.grid[yCoord][xCoord-1] == WALL){
        neighbourWalls++;
    }
    if(level.grid[yCoord][xCoord+1] == WALL){
        neighbourWalls++;
    }

    return neighbourWalls;
}

void SokoGenerator::regenerateLevel(int lvlNum){
    if(genSeed == 0){
        while(genSeed > 999999999 || genSeed == 0){
            genSeed = distribution(generator);
        }
    }
    srand(genSeed);
    genSeed = 0;
    bool generationSuccessful = false;
    Level newLevel;
    QTime timer;
    timer.start();
    clock_t start = clock();

    while(!generationSuccessful && !threadStop){
        if(isTimeout(start, 1.0f)){
            generationSuccessful = false;
        }

        newLevel.grid.clear();
        int _roomW, _roomH, _Boxes;
        QString _difficulty;
        if(noOfBoxes == 0){ _Boxes = randomNumber(3, 6); } else { _Boxes = noOfBoxes; }
        if(difficulty == 0) { _difficulty = difficulties[randomNumber(1, 5)]; } else { _difficulty = difficulties[difficulty]; }
        if(roomWidth == 0){ _roomW = randomNumber(3, 15, 3); } else { _roomW = roomWidth; }
        if(roomHeight == 0){
            if(_roomW == 3){ _roomH = randomNumber(6, 15, 3); }
            else { _roomH = randomNumber(3, 15, 3); }
        }
        else {
            _roomH = roomHeight;
        }

        initLevel(newLevel, _roomW, _roomH);
        placePatterns(newLevel, _roomW, _roomH);
        generationSuccessful = checkConnectivity(newLevel, _roomW, _roomH, _Boxes);
        if(generationSuccessful) placeGoalsAndBoxes(newLevel, _roomW, _roomH, _Boxes);
        if(generationSuccessful) placePlayer(newLevel, _roomW, _roomH);
        if(generationSuccessful){
            level lvl = LevelToCLevel(newLevel);
            struct solution sol;
            qDebug() << "Generation Started: ";
            generationSuccessful = solver.solve(lvl, timeout, sol);
            if(generationSuccessful){
                newLevel.solution = cSolToString(sol);
                diffAnalyser = new DifficultyAnalyser;
                newLevel.difficulty = diffAnalyser->calculateDifficulty(newLevel);
                if(difficulty > 0){
                    if(newLevel.difficulty != _difficulty) generationSuccessful = false;
                }
                delete diffAnalyser;
            }
            qDebug() << "Generation Successful: " << generationSuccessful;
        }

    }
    newLevel.generationTime = timer.elapsed();
    levels[lvlNum] = newLevel;
    emit regenFinished(lvlNum);
}

level SokoGenerator::LevelToCLevel(SokoGenerator::Level lvl){
    level newLevel;

    for(int column = 0; column < lvl.grid.size(); column++){
        for(int row = 0; row < lvl.grid[column].size(); row++){
            newLevel.f[column][row] = lvl.grid[column][row];
            if(newLevel.f[column][row] == '@'){
                newLevel.sx = row;
                newLevel.sy = column;
            }
            else if(newLevel.f[column][row] == '+'){
                newLevel.sx = row;
                newLevel.sy = column;
            }
        }
    }

    newLevel.width = lvl.grid[0].size();
    newLevel.height = lvl.grid.size();
    newLevel.next_level = NULL;


    return newLevel;
}

bool SokoGenerator::isTimeout(clock_t start, float timeout){
    clock_t diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    float min = msec / 1000;
    if(timeout != 0 && min >= timeout){
        return true;
    }

    return false;
}

string SokoGenerator::cSolToString(struct solution sol){
    string characters = "ludrLUDR";
    string solution;
    for(int i = 0; i < sol.length; i++){
        char sChar = sol.move[i];
        if(characters.find(sChar) != string::npos){
            solution += sChar;
        }
    }

    return solution;
}

SokoGenerator::Level SokoGenerator::calcDeadFields(SokoGenerator::Level level){
    Level deadFields = level;

    for(int y = 0; y < deadFields.grid.size(); y++){
        for(int x = 0; x < deadFields.grid[y].size(); x++){

            if(deadFields.grid[y][x] == FLOOR && neighbourCheck(deadFields, y, x) > 1){
                deadFields.grid[y][x] = DEADFIELD;
            }

        }
    }

    return deadFields;
}

void SokoGenerator::printLevel(SokoGenerator::Level level){
    QDebug debug = qDebug();

    for(int y = 0; y < level.grid.size(); y++){
        for(int x = 0; x < level.grid[y].size(); x++){
            char c = level.grid[y][x];
            debug << c;
        }
        debug << "\n";
    }
}
