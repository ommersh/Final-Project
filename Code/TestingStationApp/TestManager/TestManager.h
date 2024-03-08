#ifndef TESTMANAGER_H
#define TESTMANAGER_H
#include "ResultManager/ResultManager.h"


class TestManager {
public:
    TestManager() { m_ResultManager = ResultManager(); }
    void GetTestResults(int testId);


private:
    ResultManager m_ResultManager;
};

#endif