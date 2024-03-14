
#include <iostream>


#include "Version.h"
#include "ChronoBasedTimer.h"
#include "Factory.h"
#include "TestManager.h"
#include "MainProcess.h"
#include "CommManager.h"
#include "LocalFileCommChannelFacade.h"

int main() {
    std::cout << "Version " 
        << ProjectVersions::VERSION_MAJOR << "." 
        << ProjectVersions::VERSION_MINOR << "." 
        << ProjectVersions::VERSION_PATCH << std::endl;

    Factory *factory = Factory::getReference();
    TestManager testManager(factory->getTimer(),factory->getMemoryMonitor());
    LocalFileCommChannelFacade commChannel;
    CommManager commManager(commChannel);
    MainProcess mainProcess(&testManager,&commManager);
    mainProcess.process();
    
    return 0;
}