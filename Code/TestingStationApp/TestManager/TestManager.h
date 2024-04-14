#ifndef TESTMANAGER_H
#define TESTMANAGER_H
#include "ResultManager.h"
#include "CommManager.h"
#include <memory>

class TestManager {
public:
    TestManager();

    //Initialize the class
    void init(ResultManager& resultsManager, CommManager& commManger);

    //synchoronized access to the queue!
    void PlaceTestInQueue(TestRecipe recipe, TcaCalculation::sPointData* pointsDataArray,int arraySize);

    //Run forever, send test recipes and collect results
    void RunTestManagerProcess();

    //Check if the communication seems to be active(synchoronized)
    bool CheckCommunication();

private:
    static const int TIME_OUT_SEC = 600;

};

#endif