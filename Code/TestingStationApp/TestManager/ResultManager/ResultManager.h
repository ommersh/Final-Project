#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include <string>
#include "TestInfo.h"
#include "CommonStructures.h"
#include <string.h>

using namespace std;
class ResultManager {
public:

    void UpdateTestResult(TestResults::TestResult results);
    void UpdateTestRealTcaResult(TcaCalculation::TCA tca,int testID);
    void TestCompleted(int testID);

private:
    void printTestResults(TestResults::TestResult testResults);
    void printResult(std::string algName, TestResults::TestResult results);
    void startPrint();

};

#endif