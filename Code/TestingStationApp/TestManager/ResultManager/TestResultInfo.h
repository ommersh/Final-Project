#ifndef TESTRESULTINFO_H
#define TESTRESULTINFO_H

#include <AlgorithmEnum.h>


struct TestResultInfo {
    /*ENUM: STATUS*/
    double realTCA;
    double realDistance;
    int iterations;
    Algorithm algorithm;
    int numberOfTimePoints;
    double runTime;
    double distance;
    double TCA;
};

#endif 