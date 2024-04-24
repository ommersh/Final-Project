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

#endif