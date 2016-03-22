#ifndef SOKOGENERATOR_H
#define SOKOGENERATOR_H

#include <QObject>
#include <QThread>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include <queue>
#include <tuple>
#include <chrono>
#include "solvercpp/solver.h"

using namespace std;

#define WALL '#'
#define FLOOR ' '
#define GOAL '.'
#define BOX '$'
#define BOXONGOAL '*'
#define PLAYER '@'
#define PONGOAL '+'


class SokoGenerator : public QObject{
    Q_OBJECT
    typedef vector<vector<char>> TwoDVector_char;
    typedef vector<vector<int>> TwoDVector_int;
    typedef std::chrono::steady_clock::time_point time;
    typedef std::chrono::duration<int, std::milli> millisecs_t;


public:
    struct Level {
        TwoDVector_char grid;
        string solution;
        int generationTime;
    };

    explicit SokoGenerator(QObject *parent = 0);
    ~SokoGenerator();

    Solver solver;

    void setupForThread(QThread &thread);
    void initialSetup();
    void setRoomWidth(int value){ roomWidth = value; }
    void setRoomHeight(int value){ roomHeight = value; }
    void setBoxes(int value){ noOfBoxes = value; }
    void setLevels(int value){ noOfLevels = value; }
    void setDifficulty(int value){ difficulty = value; }
    void setPercentage(int value){ percentage = value; }
    void setTimeout(float timeLimit){ timeout = timeLimit; }
    void setRegenerate(bool value, int lvlNum) { regenLevel = value; regenLvlNum = lvlNum; }

    void updatePercentage(float value){ emit changeProgressBar(value); }
    void listLevelSet(std::vector<Level>){ for(size_t i = 0; i < levels.size(); i++){ emit addToList(i + 1); } }

    void generateLevel();
    void generateLevel(int roomWidth, int roomHeight, int noOfBoxes, int difficulty);
    void clearVectors(){ levels.clear(); }
    void regenerateLevel(int lvlNum);

    int randomNumber(int min, int max, int divisor = 1);

    void initLevel(Level &level, int roomWidth, int roomHeight);
    void placePatterns(SokoGenerator::Level &level, int roomWidth, int roomHeight);
    void rotatePattern(TwoDVector_char &pattern, int rotation);
    bool checkConnectivity(SokoGenerator::Level &level, int roomWidth, int roomHeight, int noOfBoxes);
    void placeGoalsAndBoxes(SokoGenerator::Level &level, int roomWidth, int roomHeight, int noOfBoxes);
    void placePlayer(SokoGenerator::Level &level, int roomWidth, int roomHeight);
    TwoDVector_char getLevel(int level) { return levels[level].grid; }
    vector<Level> getLevels() { return levels; }
    void floodfill(TwoDVector_int &level, int row, int column, int roomWidth, int roomHeigh);
    bool neighbourCheck(SokoGenerator::Level &level, int yCoord, int xCoord);
    level LevelToCLevel(SokoGenerator::Level lvl);

    void deleteLevel(int lvlNum){ levels.erase(levels.begin() + lvlNum-1); }

private:
    int roomWidth;
    int roomHeight;
    int noOfBoxes;
    int noOfLevels;
    int difficulty;
    int percentage;
    float timeout;
    time start;
    bool regenLevel = false;
    int regenLvlNum = 0;

    std::vector<Level> levels;
    std::vector<Level> patterns;

public:
    bool stopThread = 0;

signals:
    void changeProgressBar(float);
    void addToList(int);
    void updateTimer();
    void threadFinished();
    void regenFinished(int);
    void resetGUI();

private slots:
    void startThreadWork();
};

#endif // SOKOGENERATOR_H
