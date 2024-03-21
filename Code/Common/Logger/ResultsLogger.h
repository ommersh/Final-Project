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

    // Method to log a single row of results
    void log(TestResults::TestResult results, double TOLd, double TOLt);
};
