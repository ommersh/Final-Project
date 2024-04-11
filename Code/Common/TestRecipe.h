#ifndef TEST_RECIPE_H
#define TEST_RECIPE_H

#include <string>
#include "../TestingStationApp/Enums/SatelliteDataFormatEnum.h"
#include "Enums/AlgorithmEnum.h"
#include "../../SGP4/CPP/SGP4.h"



struct CommonTestRecipe {
    int testId;
    double timeInterval;
    int iterations;
    Algorithm alg;
    int catchPolynomDeg;
    elsetrec firstElemObj;
    elsetrec secondElemObj;
    int numOfTimePoints;
};

#endif