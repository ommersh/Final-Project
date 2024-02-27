
#include <iostream>


#include "Version.h"
#include "WinTimer.h"
#include "Factory.h"
#include "TestManager.h"
#include "MainProcess.h"

int main() {
    std::cout << "Version " 
        << ProjectVersions::VERSION_MAJOR << "." 
        << ProjectVersions::VERSION_MINOR << "." 
        << ProjectVersions::VERSION_PATCH << std::endl;

    Factory *factory = Factory::getReferance();
    TestManager testManager(factory->getTimer());
    MainProcess mainProcess(&testManager);
    mainProcess.process();
    
    return 0;
}