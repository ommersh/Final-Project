
#include <iostream>


#include "Version.h"
#include "WinTimer.h"
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

    Factory *factory = Factory::getReferance();
    TestManager testManager(factory->getTimer());
    LocalFileCommChannelFacade commChannel;
    CommManager commManager(commChannel);
    MainProcess mainProcess(&testManager,&commManager);
    mainProcess.process();
    
    return 0;
}