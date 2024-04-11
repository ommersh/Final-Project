#include "ResultsLogger.h""




// Constructors
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
            outFile << "Test Name" << ","
                    << "Test ID" << ","
                    << "Algorithm Name" << ","
                    << "Degree" << ","

                << "timeIntervalSizeSec" << ","
                << "segmentSizeSec" << ","
                << "Tmin Factor" << ","
                << "numberOfPointsPerSegment" << ","
                << "initialNumberOfPoints" << ","

                << "Number of Points" << ","
                << "runTimeSec" << ","
                << "runTimeMicro" << ","

                << "numberOfIterations" << ","
                << "avgTimeMicro" << ","
                << "minTimeMicro" << ","

                << "TCA Distance" << ","
                << "TCA Time" << ","
                << "TOLd" << ","
                << "TOLt" << "\n";
        }
        // If the file is not empty, do not write headers
    }
}


ResultsLogger::ResultsLogger() {


    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    struct tm timeinfo;

    // Convert to local time
#ifdef _WIN32
// Use localtime_s on Windows
    localtime_s(&timeinfo, &time_t);
#else
// Use localtime_r on Linux and other UNIX-like systems
    localtime_r(&time_t, &timeinfo);
#endif

    // Format the time into a string with the desired format
    std::ostringstream oss;
    oss << std::put_time(&timeinfo, "%Y-%m-%d_%H-%M-%S");
    std::string timeStr = oss.str();

    // Construct the filename using the date and time string
    std::string filename = "Logger/logger_" + timeStr + ".csv";

    std::cout << "Logger filename: " << filename << std::endl;


    m_fileName = filename;

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
            outFile << "Test Name" << ","
                << "Test ID" << ","
                << "Algorithm Name" << ","
                << "Degree" << ","

                << "timeIntervalSizeSec" << ","
                << "segmentSizeSec" << ","
                << "Tmin Factor" << ","
                << "numberOfPointsPerSegment" << ","
                << "initialNumberOfPoints" << ","

                << "Number of Points" << ","
                << "runTimeSec" << ","
                << "runTimeMicro" << ","

                << "numberOfIterations" << ","
                << "avgTimeMicro" << ","
                << "minTimeMicro" << ","

                << "TCA Distance" << ","
                << "TCA Time" << ","
                << "TOLd" << ","
                << "TOLt" << "\n";
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
void ResultsLogger::log(TestResults::TestResult results, double TOLd, double TOLt) {
    std::string algName;
    switch (results.testedAlgorithm)
    {
    case AlgorithmsEnums::ANCAS:
        algName = "ANCAS";
        TOLd = 0;
        TOLt = 0;
        break;
    case AlgorithmsEnums::CATCH:
        switch (results.catchRootsAlg)
        {
        case AlgorithmsEnums::EigenCompanionMatrix:
            algName = "CATCH_Eigen";
            break;
        case AlgorithmsEnums::ArmadilloCompanionMatrix:
            algName = "CATCH_Armadillo";
            break;
        default:
            algName = "CATCH";
            break;
        }
        TOLd = 0;
        TOLt = 0;
        break;
    case AlgorithmsEnums::SBO_ANCAS:
        algName = "SBO ANCAS";
        break;

    default:
        algName = "????";
        break;
    }
    log(results, TOLd, TOLt, algName);
}

std::string sanitize(const std::string& str) {
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    return result;
}
    // Method to log a single row of results
void ResultsLogger::log(TestResults::TestResult results,double TOLd,double TOLt, std::string algName) {
    
    if (outFile.is_open()) {
        outFile << std::fixed << std::setprecision(30) 
            << sanitize(results.testName) << ","
            << results.testID << ","
            << algName << ","
            << results.degree << ","

            << results.timeIntervalSizeSec << ","
            << results.segmentSizeSec << ","
            << 1.0/results.TminFactor << ","
            << results.numberOfPointsPerSegment << ","
            << results.initialNumberOfPoints << ","

            << results.tca.numberOfPoints << ","
            << results.runTimeMicro/ 1000000 << ","
            << results.runTimeMicro << ","

            << results.numberOfRuns << ","
            << results.avgTimeMicro << ","
            << results.minTimeMicro << ","

            << results.tca.distance << ","
            << results.tca.time << ","
            << TOLd << ","
            << TOLt << "\n";
    }
    else {
        std::cerr << "Attempt to write to a closed file." << std::endl;
    }
}
