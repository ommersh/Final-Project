#ifndef SHIELD_ResultsLogger_H    // Check if the symbol SHIELD_ResultsLogger_H is not defined
#define SHIELD_ResultsLogger_H    // Define the symbol SHIELD_ResultsLogger_H


#include <fstream>
#include <iostream>
#include <iomanip> // For std::setprecision
#include <chrono>
#include <sstream>

#include "CommonStructures.h"

class ResultsLogger {
private:
    std::ofstream outFile;
    std::string m_fileName; // Store the file name

    
public:
    // Constructors
    ResultsLogger(const std::string& filename);
    ResultsLogger();

    // Destructor
    ~ResultsLogger();

    // Methods to log a single row of results
    void log(TestResults::TestResult results, double TOLd, double TOLt, std::string algName);
    void log(TestResults::TestResult results, double TOLd, double TOLt);
};



#endif //SHIELD_ResultsLogger_H