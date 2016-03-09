#ifndef BOOSTSOLVER_H
#define BOOSTSOLVER_H

#include <iostream>
#include <QObject>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <array>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_set.hpp>
#include <chrono>

using namespace std;

typedef vector<char> TableRow;
typedef vector<TableRow> Table;
//typedef chrono::steady_clock::time_point time;
typedef chrono::duration<int, std::milli> millisecs_t;


class boostsolver : public QObject{
    Q_OBJECT
public:
    boostsolver(QObject *parent = 0);
    void board(string b);
    string solve(chrono::steady_clock::time_point t);
private:
    Table sData, dData;
    int px, py;

    bool move(int x, int y, int dx, int dy, Table &data);
    bool push(int x, int y, int dx, int dy, Table &data);
    bool isSolved(const Table &data);

signals:
    void updateTimer(float);

};

#endif // BOOSTSOLVER_H
