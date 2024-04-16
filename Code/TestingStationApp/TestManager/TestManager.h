#ifndef TESTMANAGER_H
#define TESTMANAGER_H
#include "ResultManager.h"
#include "CommManager.h"
#include "SafeQueue.h"
#include <memory>
#include <chrono>

struct TestInQueue {
    TestRecipe recipe;
    TcaCalculation::sPointData* pointsDataArray;
    int arraySize;
};
enum TestManagerState {
    eWaitingForTheNextTest,
    eWaitingForTestResults,
    eNotActive,
};

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

    void startTestManagerProcess();
    void stopTestManagerProcess();

private:
    static const unsigned long int TIME_OUT_MICROSEC = 1000000 * 60 * 10;//Ten Minutes
    static const unsigned long int SLEEP_TIME_MILLISEC = 500;//0.5 Minute

    ResultManager* m_resultsManager;
    CommManager* m_commManger;
    SafeQueue<TestInQueue> m_waitingTestQueue;  // A queue of waiting tests
    bool m_keepRunnig;
    TestManagerState m_state;
    unsigned long int m_startTime;

    unsigned long int getCurrentTimeInMicroseconds();

    std::thread m_thisThread;
};

#endif