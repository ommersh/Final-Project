#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include <string>
#include "TestInfo.h"
#include "CommonStructures.h"
class ResultManager {
public:

    void UpdateTestResult();

    void FillResults();//Fill the results in FullTestData
};

#endif