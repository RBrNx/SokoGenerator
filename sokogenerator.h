#ifndef SOKOGENERATOR_H
#define SOKOGENERATOR_H


class SokoGenerator{
public:
    SokoGenerator();
    ~SokoGenerator();

    static void setRoomWidth(int value){ roomWidth = value; }
    static void setRoomHeight(int value){ roomHeight = value; }
    static void setBoxes(int value){ noOfBoxes = value; }
    static void setLevels(int value){ noOfLevels = value; }
    static void setDifficulty(int value){ difficulty = value; }

    static int getPercentage(){ return percentage; }

    static void generateLevel();
    static void generateLevel(int noOfLevels);

private:
    static int roomWidth;
    static int roomHeight;
    static int noOfBoxes;
    static int noOfLevels;
    static int difficulty;
    static int percentage;

};

#endif // SOKOGENERATOR_H
