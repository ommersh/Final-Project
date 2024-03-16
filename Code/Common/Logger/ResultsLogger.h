#include <fstream>
#include <iostream>
#include <iomanip> // For std::setprecision

#include "CommonStructures.h"

class ResultsLogger {
private:
    std::ofstream outFile;
    std::string m_fileName; // Store the file name

    
public:
    // Constructor
    ResultsLogger(const std::string& filename);

    // Destructor
    ~ResultsLogger();

    // Method to log a single row of results
    void log(TestResults::TestResult results);
};
