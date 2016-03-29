#include "difficultyanalyser.h"

DifficultyAnalyser::DifficultyAnalyser(QObject *parent) : QObject(parent)
{

}

QString DifficultyAnalyser::calculateDifficulty(SokoGenerator::Level level){
    int pushes = calculatePushes(level.solution);
    int lines = calculateLines(level.solution);
    int boxes = calculateBoxes(level);
    int score = 100 * (pushes + (4*lines) - (12*boxes));
    int roomSize = (level.grid.size() - 2) * (level.grid[0].size() - 2);


    if(score/roomSize <= 10){
        return "Very Easy";
    }
    else if(score/roomSize >= 11 && score/roomSize <= 20){
        return "Easy";
    }
    else if(score/roomSize >= 21 && score/roomSize <= 34){
        return "Medium";
    }
    else if(score/roomSize >= 35 && score/roomSize <= 49){
        return "Hard";
    }
    else if(score/roomSize >= 50){
        return "Very Hard";
    }
    else{
        return "No Value";
    }

    //int fC = furtherCalculations(level);

    //return score/roomSize; //+= fC;
}

int DifficultyAnalyser::calculatePushes(string solution){
    string chars = "lurdLURD";
    int pushCount = 0;
    for(int i = 0; i < solution.length(); i++){
        if(chars.find(solution[i]) != string::npos && isupper(solution[i])){
            pushCount++;
        }
    }

    return pushCount;
}

int DifficultyAnalyser::calculateLines(string solution){
    string chars = "lurdLURD";
    int pushCount = 0;
    char previousChar = ' ';
    for(int i = 0; i < solution.length(); i++){
        if(chars.find(solution[i]) != string::npos && isupper(solution[i])){
            pushCount++;
            previousChar = solution[i];
            while(solution[i] == previousChar){
                solution.erase(i, 1);
            }
        }
    }

    return pushCount;
}

int DifficultyAnalyser::calculateBoxes(SokoGenerator::Level level){
    int boxCount = 0;

    for(int y = 0; y < level.grid.size(); y++){
        for(int x = 0; x < level.grid[0].size(); x++){
            if(level.grid[y][x] == BOX || level.grid[y][x] == BOXONGOAL){
                boxCount++;
            }
        }
    }

    return boxCount;
}

int DifficultyAnalyser::neighbourCheck(SokoGenerator::Level Level, int x, int y, char first, char second){
    int count = 0;

    if(Level.grid[y-1][x] == first || Level.grid[y-1][x] == second){
        count++;
    }
    if(Level.grid[y+1][x] == first || Level.grid[y+1][x] == second){
        count++;
    }
    if(Level.grid[y][x-1] == first || Level.grid[y][x-1] == second){
        count++;
    }
    if(Level.grid[y][x+1] == first || Level.grid[y][x+1] == second){
        count++;
    }

    return count;
}

int DifficultyAnalyser::furtherCalculations(SokoGenerator::Level level){
    int neighbourBoxes = 0;
    int neighbourWalls = 0;
    int neighbourPlayer = 0;
    int neighbourGoals = 0;

    for(int y = 0; y < level.grid.size(); y++){
        for(int x = 0; x < level.grid[0].size(); x++){

            if(level.grid[y][x] == BOX){
                if(neighbourCheck(level, x, y, BOX, BOXONGOAL) > 0){
                    neighbourBoxes++;
                }
                if(neighbourCheck(level, x, y, WALL) > 0){
                    neighbourWalls++;
                }
                if(neighbourCheck(level, x, y, PLAYER, PONGOAL) > 0){
                    neighbourPlayer++;
                }
            }

            if(level.grid[y][x] == GOAL){
                if(neighbourCheck(level, x, y, GOAL, PONGOAL) > 0){
                    neighbourGoals++;
                }
            }
        }
    }

    return (neighbourBoxes * 30) + (neighbourWalls * -150) + (neighbourPlayer * 50) + (neighbourGoals * 30);
}

