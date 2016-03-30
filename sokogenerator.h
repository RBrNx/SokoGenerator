#ifndef SOKOGENERATOR_H
#define SOKOGENERATOR_H

#include <QObject>
#include <QThread>
#include <QString>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include <queue>
#include <tuple>
#include <chrono>
#include <random>
#include "solvercpp/solver.h"

class DifficultyAnalyser;

using namespace std;

#define WALL '#'
#define FLOOR ' '
#define GOAL '.'
#define BOX '$'
#define BOXONGOAL '*'
#define PLAYER '@'
#define PONGOAL '+'
#define DEADFIELD 'x'


class SokoGenerator : public QObject{
    Q_OBJECT
    typedef vector<vector<char>> TwoDVector_char;
    typedef vector<vector<int>> TwoDVector_int;
    typedef unsigned long long ull;


public:
    struct Level {
        TwoDVector_char grid;
        string solution;
        int generationTime;
        QString difficulty;
    };

    explicit SokoGenerator(QObject *parent = 0);
    ~SokoGenerator();

    Solver solver;
    DifficultyAnalyser* diffAnalyser;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;

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
    void setGenerateSeed(int seed){ genSeed = seed; }

    void updatePercentage(float value){ emit changeProgressBar(value); }
    void listLevelSet(std::vector<Level>){ for(size_t i = 0; i < levels.size(); i++){ emit addToList(i + 1); } }

    void generateLevel();
    void generateLevel(int roomWidth, int roomHeight, int noOfBoxes, int difficulty);
    void clearVectors(){ levels.clear(); }
    void regenerateLevel(int lvlNum);

    ull randomNumber(ull min, ull max, int divisor = 1);

    void initLevel(Level &level, int roomWidth, int roomHeight);
    void placePatterns(SokoGenerator::Level &level, int roomWidth, int roomHeight);
    void rotatePattern(TwoDVector_char &pattern, int rotation);
    bool checkConnectivity(SokoGenerator::Level &level, int roomWidth, int roomHeight, int noOfBoxes);
    bool placeGoalsAndBoxes(SokoGenerator::Level &level, int roomWidth, int roomHeight, int noOfBoxes);
    bool placePlayer(SokoGenerator::Level &level, int roomWidth, int roomHeight);
    TwoDVector_char getLevel(int level) { return levels[level].grid; }
    vector<Level> getLevels() { return levels; }
    int getGenSeed(){ return genSeed; }
    void floodfill(TwoDVector_int &level, int row, int column, int roomWidth, int roomHeigh);
    int neighbourCheck(SokoGenerator::Level &level, int yCoord, int xCoord);
    level LevelToCLevel(SokoGenerator::Level lvl);
    string cSolToString(struct solution sol);
    Level calcDeadFields(SokoGenerator::Level level);
    void printLevel(SokoGenerator::Level level);

    void deleteLevel(int lvlNum){ levels.erase(levels.begin() + lvlNum); }
    bool isTimeout(clock_t start, float timeout);

private:
    int roomWidth;
    int roomHeight;
    int noOfBoxes;
    int noOfLevels;
    int difficulty;
    int percentage;
    int genSeed;
    float timeout;
    bool regenLevel = false;
    int regenLvlNum = 0;

    QString difficulties[6]  = { "None", "Very Easy", "Easy", "Medium", "Hard", "Very Hard" };

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
    void displayGenSeed();

private slots:
    void startThreadWork();
};

#endif // SOKOGENERATOR_H
