
#ifndef TESTINFO_H
#define TESTINFO_H

#include "../../Enums/TestStatus.h"
#include "../../../Common/TestRecipe.h"



struct TestInfo {
    char name[40];
    TestStatus status;
    CommonTestRecipe recipe;

    //STRUCT TCA exist
    double realTCA;
    double realDistance;
    //struct testResult
    double distance;
    double TCA;
    double runTime;
    SatelliteDataFormat format;
    char firstElemData[80]; //send elstrac
    char secondElemData[80];

};

#endif 