#include <iostream>

#include "Version.h"
#include "ChronoBasedTimer.h"
#include "Factory.h"
#include "TestManager.h"
#include "MainProcess.h"
#include "CommManager.h"
#include "Utilities.h"
#include "EventLogger.h"




bool g_keepRunning = true;

#ifndef _WIN32
#include <csignal>
void signalHandler(int signum) {
    g_keepRunning = false;
}
#endif

int main() {

#ifndef _WIN32
    std::signal(SIGINT, signalHandler);
#endif


    std::cout << "Version " 
        << ProjectVersions::VERSION_MAJOR << "." 
        << ProjectVersions::VERSION_MINOR << "." 
        << ProjectVersions::VERSION_PATCH << std::endl;

    EventLogger::getInstance().log("App Started", "main");
    std::string versionInfo = "App Version: " + 
        std::to_string(ProjectVersions::VERSION_MAJOR) + "." + 
        std::to_string(ProjectVersions::VERSION_MINOR) + "." +
        std::to_string(ProjectVersions::VERSION_PATCH);
    EventLogger::getInstance().log(versionInfo, "main");

    Factory *factory = Factory::GetReference();
    EventLogger::getInstance().log("Factory created", "main");

    TestManager testManager(factory->GetTimer());
    CommManager commManager(*factory->GetCommChannel());
    commManager.Init();
    MainProcess mainProcess(&testManager,&commManager, factory->GetResultsLogger());
    mainProcess.Process();
    
    return 0;
}