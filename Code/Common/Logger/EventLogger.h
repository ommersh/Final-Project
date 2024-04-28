#ifndef SHIELD_EventLogger_H    // Check if the symbol SHIELD_EventLogger_H is not defined
#define SHIELD_EventLogger_H    // Define the symbol SHIELD_EventLogger_H


#include <fstream>
#include <iostream>
#include <iomanip> // For std::setprecision
#include <chrono>
#include <sstream>
#include <string>
#include <mutex>

class EventLogger {
private:
    std::ofstream outFile;
    std::string m_fileName; // Store the file name
    mutable std::mutex mtx;

    // Constructors
    EventLogger()
    {
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
        oss << std::put_time(&timeinfo, "%d-%m-%Y_%H-%M-%S");
        std::string timeStr = oss.str();

        // Construct the filename using the date and time string
        std::string filename = "Logger/EventsLog_" + timeStr + ".Log";

        std::cout << "Events Log filename: " << filename << std::endl;

        m_fileName = filename;

        outFile.open(filename, std::ios::app); // Append mode
        if (!outFile.is_open()) {
            std::cout << "Failed to open " << filename << " for writing." << std::endl;
        }
        else {
            std::cout << "Opened " << filename << " for writing." << std::endl;
        }
    }

    // Destructor
    ~EventLogger()
    {
        if (outFile.is_open()) {
            std::cout << "Closing Events Log file: " << m_fileName << std::endl;
            outFile.close();
        }
    }
public:

    static EventLogger& getInstance()
    {
        static EventLogger instance;
        return instance;
    }

    /// <summary>
    /// Log envent in the format:
    /// Date::Time - Log::EventHolder::Event
    /// </summary>
    /// <param name="Event"></param>
    /// <param name="EventHolder"></param>
    void log(std::string Event, std::string EventHolder)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (outFile.is_open()) {
            //Build the time string
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
            oss << std::put_time(&timeinfo, "%d/%m/%Y-%H:%M:%S");
            std::string timeStr = oss.str();

            outFile << timeStr << "::" << EventHolder << "::" << Event << std::endl;
            outFile.flush();
        }
    }
};



#endif //SHIELD_EventLogger_H