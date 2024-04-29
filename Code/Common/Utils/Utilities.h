#ifndef Utilities_H
#define Utilities_H

#include <cstdint>
#include <array>

/// <summary>
/// static class for calculating 4 bytes crc
/// </summary>
class CRC32 {
public:
    /// <summary>
    /// Calculate CRC32
    /// </summary>
    /// <param name="data">Calculate crc for this buffer</param>
    /// <param name="size">The buffer size</param>
    /// <returns></returns>
    static uint32_t calculate(const unsigned char* data, size_t size) {
        static const std::array<uint32_t, 256> table = generateTable();
        uint32_t crc = 0xFFFFFFFF;
        for (size_t i = 0; i < size; i++) {
            uint8_t index = (crc ^ data[i]) & 0xFF;
            crc = (crc >> 8) ^ table[index];
        }
        return ~crc;
    }

private:
    // Generate CRC lookup table as a static function
    static std::array<uint32_t, 256> generateTable() {
        std::array<uint32_t, 256> table;
        const uint32_t polynomial = 0xEDB88320;
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t crc = i;
            for (uint32_t j = 0; j < 8; j++) {
                crc = (crc & 1) ? (crc >> 1) ^ polynomial : crc >> 1;
            }
            table[i] = crc;
        }
        return table;
    }

};

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
/// <summary>
/// Log class for a buffer(in hexa), can be used for debug
/// </summary>
class BufferLog
{
public:
    BufferLog(std::string fileName) {
        // Open the file stream for output, append mode
        outFile.open(fileName, std::ios::out | std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open file: " << fileName << std::endl;
        }
    }
    ~BufferLog() {
        if (outFile.is_open()) {
            outFile.close();  // Ensure the file is closed when the object is destroyed
        }
    }
    //write the buffer to the filse as hexa
    void writeToLog(unsigned char* buffer, unsigned int size) {
        if (!outFile.is_open()) {
            std::cerr << "File is not open, cannot write to log." << std::endl;
            return;
        }

        // Set formatting for hexadecimal output
        outFile << std::hex << std::uppercase << std::setfill('0');
        for (unsigned int i = 0; i < size; ++i) {
            outFile << std::setw(2) << static_cast<int>(buffer[i]) << " ";
            if ((i + 1) % 32 == 0) { // After every 32 bytes, insert a newline
                outFile << std::endl;
            }
        }
        outFile << std::dec << std::endl;  // Switch back to decimal and add a newline
    }
private:
    std::ofstream outFile;  // File stream object
};

inline unsigned int getMin(unsigned int a, unsigned int b) {
    return (a < b) ? a : b;
}

#include <map>
class IDTracker {
private:
    std::map<int, int> inProgressTasks;

public:
    // Function to mark an ID as completed
    void markInProgress(int id) {
        if (inProgressTasks.find(id) == inProgressTasks.end()) {
            inProgressTasks[id] = 1;
        }
        else {
            inProgressTasks[id]++;
        }
    }

    // Function to check if an ID is completed
    bool isInProgress(int id) {
        return inProgressTasks.find(id) != inProgressTasks.end();
    }

    void markCompleted(int id) {
        auto it = inProgressTasks.find(id);
        if (it != inProgressTasks.end()) {
                inProgressTasks.erase(it);
        }
    }

};
#ifdef WIN32
#include "TestRecipe.h";
class CatalogReader
{

public:
    bool Init(const std::string& catalogFilePath)
    {
        m_inputFile.open(catalogFilePath);

        if (!m_inputFile.is_open()) {
            std::cout << "Failed to open catalog file: " << catalogFilePath << std::endl;
            return false;
        }
        m_catalogSize = getCatalogSize();
        if (m_catalogSize < 2)
        {
            std::cout << "The Catalog Size is too small" << std::endl;
            return false;
        }
        return true;
    }
    //The input file
    std::ifstream m_inputFile;
    std::string m_tleLine1;
    std::string m_tleLine2;
    std::string m_name;
    char m_testName[MAX_TEST_NAME_SIZE];
    int m_catalogSize;
    int getCatalogSize()
    {
        if (!m_inputFile.is_open()) {
            std::cerr << "File is not open" << std::endl;
            return 0;
        }

        m_inputFile.clear();  // Clear any error flags
        m_inputFile.seekg(0, std::ios::beg);  // Seek to the beginning of the file

        std::string line;
        int lineCount = 0;
        int satelliteCount = 0;
        while (getline(m_inputFile, line)) {
            if (lineCount++ % 3 == 2) {
                satelliteCount++;

            }
        }
        return satelliteCount;
    }
    bool getTLE(int position)
    {
        if (!m_inputFile.is_open()) {
            std::cerr << "File is not open" << std::endl;
            return false;
        }

        m_inputFile.clear();  // Clear any error flags
        m_inputFile.seekg(0, std::ios::beg);  // Seek to the beginning of the file

        std::string line;
        int lineCount = 0;
        int satelliteCount = 0;

        while (getline(m_inputFile, line)) {


            if (lineCount++ % 3 == 0) {
                if (satelliteCount++ == position) {
                    // Copy the satellite name to the global variable
                    m_name = line;
                    if (getline(m_inputFile, line)) {
                        m_tleLine1 = line;
                        if (getline(m_inputFile, line)) {
                            m_tleLine2 = line;
                        }
                        else {
                            std::cerr << "Failed to read TLE line 2" << std::endl;
                            return false;
                        }
                    }
                    else {
                        std::cerr << "Failed to read TLE line 1" << std::endl;
                        return false;
                    }
                    return true;
                }
            }
        }

        if (satelliteCount < position) {
            std::cerr << "Position exceeds number of satellites in file" << std::endl;
            return false;
        }
    }
    void printPercentage(int position , int catalogSize)
    {
        double percentage = static_cast<double>(position) / catalogSize * 100.0;

        // Move cursor to the beginning of the line with '\r'
        // Set text color to green with ANSI escape code "\033[32m"
        // Reset text color to default with "\033[0m" at the end
        std::cout << "\r\033[32m" // Set text color to green
            << std::fixed << std::setprecision(10) << percentage << "%"
            << "\033[0m" // Reset text color to default
            << std::flush; // Flush the stream to ensure immediate output
    }
    void parseTestName(std::string n1, std::string n2)
    {
        std::string tempName;
        tempName = n1;  // Copying n1 to tempName
        tempName += '_';    // Appending '_' to tempName
        tempName += n2;  // Append n2 to tempName

        tempName.erase(std::remove(tempName.begin(), tempName.end(), ' '), tempName.end());
        tempName.erase(std::remove(tempName.begin(), tempName.end(), '\t'), tempName.end());
        tempName.erase(std::remove(tempName.begin(), tempName.end(), '\n'), tempName.end());

        //Get the test name
        strncpy(m_testName, tempName.c_str(), MAX_TEST_NAME_SIZE - 1);
    }
};
#endif //WIN32

#endif