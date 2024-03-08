#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include <string>

class ResultManager {
public:
    void handleTestResult();
    void saveData(const std::string& path);
};

#endif