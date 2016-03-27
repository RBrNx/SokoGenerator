#ifndef DIFFICULTYANALYSER_H
#define DIFFICULTYANALYSER_H

#include <QObject>
#include <string>
#include "sokogenerator.h"

using namespace std;

class DifficultyAnalyser : public QObject
{
    Q_OBJECT
public:
    explicit DifficultyAnalyser(QObject *parent = 0);
    int calculateDifficulty(SokoGenerator::Level level);

private:
    int calculatePushes(string solution);
    int calculateLines(string solution);
    int calculateBoxes(SokoGenerator::Level level);
    int neighbourCheck(SokoGenerator::Level level, int x, int y, char first, char second = NULL);
    int furtherCalculations(SokoGenerator::Level level);


signals:

public slots:
};

#endif // DIFFICULTYANALYSER_H
