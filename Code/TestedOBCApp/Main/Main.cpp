
#include <iostream>


#include "Version.h"
#include "ChronoBasedTimer.h"
#include "Factory.h"
#include "TestManager.h"
#include "MainProcess.h"
#include "CommManager.h"
#include "LocalFileCommChannelFacade.h"





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

   
    ResultsLogger resultsLogger;
    Factory *factory = Factory::getReference();
    TestManager testManager(factory->getTimer());
    LocalFileCommChannelFacade commChannel;
    CommManager commManager(commChannel);
    MainProcess mainProcess(&testManager,&commManager, &resultsLogger);
    mainProcess.process();
    
    return 0;
}