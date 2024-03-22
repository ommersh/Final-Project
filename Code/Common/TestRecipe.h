#ifndef TEST_RECIPE_H
#define TEST_RECIPE_H

#include <string>
#include "../TestingStationApp/Enums/SatelliteDataFormatEnum.h"
#include <AlgorithmEnum.h>


struct TestRecipe {
    std::string name;
    unsigned int testId;
    std::string description;
    SatelliteDataFormat format;
    double timeInterval;
    int iterations;
    double propogatorDeltaTime;
    Algorithm alg;
    int catchPolynomDeg;
    std::string firstElemData;
    std::string secondElemData;

};

#endif