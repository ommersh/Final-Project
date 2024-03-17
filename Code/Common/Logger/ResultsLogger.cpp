#include "ResultsLogger.h""

// Constructor
ResultsLogger::ResultsLogger(const std::string& filename) : m_fileName(filename) {
    outFile.open(filename, std::ios::app); // Append mode
    if (!outFile.is_open()) {
        std::cout << "Failed to open " << filename << " for writing." << std::endl;
    }
    else {
        std::cout << "Opened " << filename << " for writing." << std::endl;

        // Check if the file is new or empty by checking its size
        outFile.seekp(0, std::ios::end); // Move the write pointer to the end of the file
        if (outFile.tellp() == 0) { // If the write pointer is at position 0, the file is empty
            // File is new or empty, write the headers
            outFile << "Test Name,Test ID,Algorithm Name,Degree,Number of Points,Run Time (s),Run Time (us),Number Of Runs,Average Run Time,,Minimum Run Time,TCA Distance,TCA Time\n";
        }
        // If the file is not empty, do not write headers
    }
}

// Destructor
ResultsLogger::~ResultsLogger() {
    if (outFile.is_open()) {
        std::cout << "Closing the results file: " << m_fileName << std::endl;
        outFile.close();
    }
}

    // Method to log a single row of results
void ResultsLogger::log(TestResults::TestResult results) {
    std::string algName;
    switch (results.testedAlgorithm)
    {
    case TestParameters::ANCAS:
        algName = "ANCAS";
        break;
    case TestParameters::CATCH:
        switch (results.catchRootsAlg)
        {
        case TestParameters::EigenCompanionMatrix:
            algName = "CATCH_Eigen";
            break;
        case TestParameters::ArmadilloCompanionMatrix:
            algName = "CATCH_Armadillo";
            break;
        default:
            algName = "CATCH";
            break;
        }
        break;
    case TestParameters::SBO_ANCAS:
        algName = "SBO ANCAS";
        break;

    default:
        algName = "????";
        break;
    }
    if (outFile.is_open()) {
        outFile << std::fixed << std::setprecision(15) 
            << results.testName << ","
            << results.testID << ","
            << algName << ","
            << results.degree << ","
            << results.tca.numberOfPoints << ","
            << results.runTimeMicro/ 1000000 << ","
            << results.runTimeMicro << ","

            << results.numberOfRuns << ","
            << results.avgTimeMicro << ","
            << results.minTimeMicro << ","

            << results.tca.distance << ","
            << results.tca.time << "\n";
    }
    else {
        std::cerr << "Attempt to write to a closed file." << std::endl;
    }
}
