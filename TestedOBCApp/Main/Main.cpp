
#include <iostream>


#include "Version.h"
#include "WinTimer.h"
#include "MainProcess.h"


















int main() {
    std::cout << "Version " 
        << ProjectVersions::VERSION_MAJOR << "." 
        << ProjectVersions::VERSION_MINOR << "." 
        << ProjectVersions::VERSION_PATCH << std::endl;

    WinTimer timer;
    MainProcess mainProcess(timer);
    mainProcess.process();
    
    return 0;
}