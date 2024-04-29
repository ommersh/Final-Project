#ifndef SHIELD_TestResultsToCsv_H    // Check if the symbol SHIELD_TestResultsToCsv_H is not defined
#define SHIELD_TestResultsToCsv_H    // Define the symbol SHIELD_TestResultsToCsv_H


#include <fstream>
#include <iostream>
#include <iomanip> // For std::setprecision
#include <chrono>
#include <sstream>
#include <algorithm>
#include <string>
#include "TestInfo.h"

class TestResultsToCsv {
private:
    std::ofstream outFile;
    std::string m_fileName; // Store the file name
    void SetFileHeader();
    std::string sanitize(const std::string& str);
public:
    // Constructor
    TestResultsToCsv();

    // Destructor
    ~TestResultsToCsv();

    // Methods to save a single test
    void SaveInfo(TestInfo testInfo, std::string algName, int FullTolReached, int ToldReached, int ToltReached, int NoRootsFound);
    void SaveInfo(TestInfo testInfo, int FullTolReached, int ToldReached, int ToltReached, int NoRootsFound);
};



#endif //SHIELD_TestResultsToCsv_H
