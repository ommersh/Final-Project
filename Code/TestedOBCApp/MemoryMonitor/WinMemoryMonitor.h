#ifndef SHIELD_WinMemoryMonitor_H    // Check if the symbol SHIELD_WinMemoryMonitor_H is not defined
#define SHIELD_WinMemoryMonitor_H    // Define the symbol SHIELD_WinMemoryMonitor_H

#include "IMemoryMonitor.h"

class WinMemoryMonitor : public IMemoryMonitor {
private:
    long long peakMemoryUsage = 0;

public:
    virtual void startMonitoring() override;

    virtual long long stopMonitoringAndGetPeakUsage() override;

};
#endif //SHIELD_WinMemoryMonitor_H