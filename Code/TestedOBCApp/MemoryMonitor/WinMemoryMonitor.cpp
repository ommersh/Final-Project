#include "WinMemoryMonitor.h"
#include <windows.h>
#include <psapi.h>
#include <iostream>


void WinMemoryMonitor::startMonitoring() {
    // Reset peak memory usage
    peakMemoryUsage = 0;
    // On Windows, there's not a specific start action needed for snapshot-based monitoring
}

long long WinMemoryMonitor::stopMonitoringAndGetPeakUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        peakMemoryUsage = static_cast<long long>(pmc.PeakWorkingSetSize);
    }
    return peakMemoryUsage;
}
