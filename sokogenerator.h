#ifndef SOKOGENERATOR_H
#define SOKOGENERATOR_H

#include <QObject>


class SokoGenerator : public QObject{
    Q_OBJECT

public:
    explicit SokoGenerator(QObject *parent = 0);
    ~SokoGenerator();

    void setRoomWidth(int value){ roomWidth = value; }
    void setRoomHeight(int value){ roomHeight = value; }
    void setBoxes(int value){ noOfBoxes = value; }
    void setLevels(int value){ noOfLevels = value; }
    void setDifficulty(int value){ difficulty = value; }
    void setPercentage(int value){ percentage = value; }

    void resetVariables(){ roomHeight = roomWidth = noOfBoxes = noOfLevels = difficulty = 0; }

    int getPercentage(){ return percentage; }
    void updatePercentage(float value);

    void generateLevel();
    void generateLevel(int roomWidth, int roomHeight, int noOfBoxes, int difficulty, int levelNumber);

    int randomNumber(int min, int max);

private:
    int roomWidth;
    int roomHeight;
    int noOfBoxes;
    int noOfLevels;
    int difficulty;
    int percentage;

signals:
    void changeProgressBar(float);

};

#endif // SOKOGENERATOR_H
