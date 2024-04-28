#ifndef TESTMANAGER_H
#define TESTMANAGER_H
#include "ResultManager.h"
#include "CommManager.h"
#include "SafeQueue.h"
#include <memory>
#include <chrono>

/// <summary>
/// Holds a test while in queue
/// </summary>
struct TestInQueue {
    TestRecipe recipe;
    TcaCalculation::sPointData* pointsDataArray;
    int arraySize;
};
/// <summary>
/// Test manager state for inner state machine
/// </summary>
enum TestManagerState {
    eWaitingForTheNextTest,
    eWaitingForTestResults,
    eNotActive,
};

/// <summary>
/// Manage the outgoing tests and incoming results.
/// </summary>
class TestManager {
public:
    TestManager();

    /// <summary>
    /// Initialize the class
    /// </summary>
    /// <param name="resultsManager"></param>
    /// <param name="commManger"></param>
    void init(ResultManager& resultsManager, CommManager& commManger);

    /// <summary>
    /// Place a test in the outgoing tests queue
    /// </summary>
    /// <param name="recipe"></param>
    /// <param name="pointsDataArray"></param>
    /// <param name="arraySize"></param>
    void PlaceTestInQueue(TestRecipe recipe, TcaCalculation::sPointData* pointsDataArray,int arraySize);

    /// <summary>
    /// Run forever, send test recipes and collect results
    /// </summary>
    void RunTestManagerProcess();

    /// <summary>
    /// Check if the communication seems to be active(synchoronized)
    /// </summary>
    /// <returns></returns>
    bool CheckCommunication();

    void startTestManagerProcess();
    void stopTestManagerProcess();

private:
    static const unsigned long int TIME_OUT_MICROSEC = 1000000 * 60 * 10;//Ten Minutes
    static const unsigned long int SLEEP_TIME_MILLISEC = 500;//0.5 Seconds

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