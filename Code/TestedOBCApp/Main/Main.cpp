
#include <iostream>


#include "Version.h"
#include "ChronoBasedTimer.h"
#include "Factory.h"
#include "TestManager.h"
#include "MainProcess.h"
#include "CommManager.h"
#include "LocalFileCommChannelFacade.h"
#include <chrono>

int main() {
    std::cout << "Version " 
        << ProjectVersions::VERSION_MAJOR << "." 
        << ProjectVersions::VERSION_MINOR << "." 
        << ProjectVersions::VERSION_PATCH << std::endl;

    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    struct tm timeinfo;

    // Convert to local time
    localtime_s(&timeinfo, &time_t);

    // Format the time into a string with the desired format
    std::ostringstream oss;
    oss << std::put_time(&timeinfo, "%Y-%m-%d_%H-%M-%S");
    std::string timeStr = oss.str();

    // Construct the filename using the date and time string
    std::string filename = "Logger/logger_" + timeStr + ".csv";

    std::cout << "Logger filename: " << filename << std::endl;

    ResultsLogger resultsLogger(filename);



    Factory *factory = Factory::getReference();
    TestManager testManager(factory->getTimer());
    LocalFileCommChannelFacade commChannel;
    CommManager commManager(commChannel);
    MainProcess mainProcess(&testManager,&commManager, &resultsLogger);
    mainProcess.process();
    
    return 0;
}