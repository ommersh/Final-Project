#ifndef SHIELD_IMemoryMonitor_H    // Check if the symbol SHIELD_IMemoryMonitor_H is not defined
#define SHIELD_IMemoryMonitor_H    // Define the symbol SHIELD_IMemoryMonitor_H


class IMemoryMonitor {
public:
    // Virtual destructor for proper cleanup of derived classes
    virtual ~IMemoryMonitor() {}

    // Starts monitoring memory usage
    virtual void startMonitoring() = 0;

    // Stops monitoring memory usage and returns the peak memory usage during the monitoring period
    virtual long long stopMonitoringAndGetPeakUsage() = 0;
};

#endif //SHIELD_IMemoryMonitor_H