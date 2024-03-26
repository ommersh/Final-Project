#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include <string>
#include "TestInfo.h"

class ResultManager {
public:
    TestInfo GetTestResult(int testId);
    void SaveData(const std::string& path);
    void UpdateTestResult(TestInfo testInfo);
};

#endif