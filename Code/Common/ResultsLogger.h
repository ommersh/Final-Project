#include <fstream>
#include <iostream>
#include <iomanip> // For std::setprecision

class ResultsLogger {
private:
    std::ofstream outFile;
    std::string m_fileName; // Store the file name

    
public:
    // Constructor
    ResultsLogger(const std::string& filename) : m_fileName(filename) {
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
                outFile << "Test Name,Algorithm Name,Degree,Number of Points,Run Time (s),Run Time (us),Max Memory (KB),Distance,Time\n";
            }
            // If the file is not empty, do not write headers
        }
    }

    // Destructor
    ~ResultsLogger() {
        if (outFile.is_open()) {
            std::cout << "Closing the results file: " << m_fileName << std::endl;
            outFile.close();
        }
    }

    // Method to log a single row of results
    void log(const std::string& testName, const std::string& algName, int degree, int numberOfPoints,
        double runTimeSec, long long runTimeMicro, double maxMemoryKB, double distance, double time) {
        if (outFile.is_open()) {
            outFile << std::fixed << std::setprecision(15) << testName << ","
                << algName << ","
                << degree << ","
                << numberOfPoints << ","
                << runTimeSec << ","
                << runTimeMicro << ","
                << maxMemoryKB << ","
                << distance << ","
                << time << "\n";
        }
        else {
            std::cerr << "Attempt to write to a closed file." << std::endl;
        }
    }
};
