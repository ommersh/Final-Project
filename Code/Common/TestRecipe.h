#ifndef TEST_RECIPE_H
#define TEST_RECIPE_H

#include <string>
#include "../TestingStationApp/Enums/SatelliteDataFormatEnum.h"
#include <AlgorithmEnum.h>


struct CommonTestRecipe {
    unsigned int testId;
    SatelliteDataFormat format;
    double timeInterval;
    int iterations;
    Algorithm alg;
    int catchPolynomDeg;
    std::string firstElemData;
    std::string secondElemData;
    int numOfTimePoints;
};

#endif