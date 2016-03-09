#include "sokosolver.h"

SokoSolver::SokoSolver()
{
    Env = createVM(&JVM);
    callSolverMethod(Env);
}

JNIEnv* SokoSolver::createVM(JavaVM **JVM){
    JNIEnv* Env;
    JavaVMInitArgs args;
    JavaVMOption options;

    jsize nVMs;
    JNI_GetCreatedJavaVMs(NULL, 0, &nVMs);
    cout << "nVMs: " << nVMs << endl;

    //options.optionString = "-Djava.class.path=./";
    options.optionString = "-Djava.class.path=C:\\Users\\RBrNx\\Documents\\Qt Projects\\SokoGenerator\\java";

    args.version = JNI_VERSION_1_6;
    args.nOptions = 1;
    args.options = &options;
    args.ignoreUnrecognized = 0;
    int returnValue = JNI_CreateJavaVM(JVM, (void**)&Env, &args);
    if(returnValue < 0 || !Env){
        cout << "Unable to launch JVM, Return Value: " << returnValue << endl;
    }
    else{
        cout << "JVM Successful!" << endl;
    }

    return Env;
}

void SokoSolver::callSolverMethod(JNIEnv* Env/*, string className, string methodName, string returnMethod*/){
    jclass JavaClass;
    jmethodID IntMethod;
    jmethodID BoolMethod;
    jmethodID printMethod;

    jint Int;
    jboolean Bool;

    JavaClass = Env->FindClass("helloWorld");
    if(JavaClass == nullptr) { cout << "ERROR: JavaClass not found" << endl; }
    IntMethod = Env->GetStaticMethodID(JavaClass, "intMethod", "(I)I");
    if(IntMethod == nullptr) { cout << "ERROR: IntMethod not found" << endl; }
    BoolMethod = Env->GetStaticMethodID(JavaClass, "booleanMethod", "(Z)Z");
    if(BoolMethod == nullptr) { cout << "ERROR: BoolMethod not found" << endl; }
    printMethod = Env->GetStaticMethodID(JavaClass, "printMethod", "()V");

    Int = Env->CallStaticIntMethod(JavaClass, IntMethod, 5);
    Bool = Env->CallStaticBooleanMethod(JavaClass, BoolMethod, 0);
    Env->CallStaticVoidMethod(JavaClass, printMethod);

    cout << "Returned Int: " << Int << endl;
    cout << "Returned Bool: " << (bool)Bool << endl;

    //JVM->DestroyJavaVM();

}


