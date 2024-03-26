
#ifndef TESTINFO_H
#define TESTINFO_H

#include "AlgorithmEnum.h"
#include "TestRecipe.h"
#include "../../Enums/TestStatus.h"



struct TestInfo {
    TestStatus status;
    CommonTestRecipe recipe;
    std::string name;
    double realTCA;
    double realDistance;
    double distance;
    double TCA;
    double runTime;
    std::string description;

};

#endif 