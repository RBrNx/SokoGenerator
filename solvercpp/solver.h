#ifndef SOLVER_H
#define SOLVER_H

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <QOBject>
#include <QDebug>
#include <chrono>

extern "C" {
#include "level.h"
#include "level_info.h"
#include "position.h"
#include "global.h"
#include "allocator.h"
#include "solving_routine.h"
#include "deadlock_table.h"
}

typedef std::chrono::duration<int, std::milli> millisecs_t;

class Solver : public QObject{
    Q_OBJECT
public:
    Solver(QObject *parent = 0);
    bool solve(level Level, float t, struct solution &sol);
    void setThreadStop(bool value) { threadStop = value; }
private:
    void print_binary(unsigned int x);
signals:
    void updateTimer(float);
};

#endif // SOLVER_H
