#ifndef SOKOSOLVER_H
#define SOKOSOLVER_H

#include <jni.h>
#include <string>
#include <iostream>

using namespace std;


class SokoSolver
{
public:
    SokoSolver();
    JNIEnv* createVM(JavaVM **JVM);
    void callSolverMethod(JNIEnv* env);

private:
    JavaVM *JVM;
    JNIEnv *Env;


};

#endif // SOKOSOLVER_H
