
#include <iostream>


#include "Version.h"
#include "ChronoBasedTimer.h"
#include "Factory.h"
#include "TestManager.h"
#include "MainProcess.h"
#include "CommManager.h"





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

   

    Factory *factory = Factory::getReference();
    TestManager testManager(factory->getTimer());
    CommManager commManager(*factory->getCommChannel());
    commManager.init();
    MainProcess mainProcess(&testManager,&commManager, factory->getResultsLogger());
    mainProcess.process();
    
    return 0;
}