#ifndef TESTMANAGER_H
#define TESTMANAGER_H
#include "ResultManager/ResultManager.h"
#include <TestDataGenerationManager.h>


class TestManager {
public:
    TestManager();
    void GetTestResults(int testId);
    ResultManager m_ResultManager;
    TestDataGenerationManager m_DataGenerator;


private:

};

#endif