#include "PCinfo.h"
#include "consoleView.h"

#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <iomanip>

using namespace std;

static const float GB = 1024 * 1024 * 1024.0f;

unsigned long long FileTimeToInt64(const FILETIME& ft);

string getUsernameInfo(){
    DWORD size = 0;
    GetUserNameA(NULL, &size);
    if (size == 0) return "Unknow";

    string name(size, '\0');
    GetUserNameA(&name[0], &size);
    if (!name.empty()) name.resize(size - 1);

    return name;
}

void getRAMInfo(){
    MEMORYSTATUSEX memInfo;

    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) {
        double total = (double)memInfo.ullTotalPhys / GB;
        double load = (double)memInfo.ullAvailPhys / GB;
        double used = total - load;
        double loadPercent = (double)memInfo.dwMemoryLoad;

        renderMemoryInfo(loadPercent, used, total);
    } else {
        cerr << "Error getting memory status. Error code: " << GetLastError() << endl;
    }
}

void getCPUInfo(){
    static unsigned long long preIdle = 0;   
    static unsigned long long preKernel = 0;
    static unsigned long long preUser = 0;
    static bool firstCall = true;

    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        cerr << "Error getting CPU times" << endl;
        return;
    }

    unsigned long long currentIdle = FileTimeToInt64(idleTime);
    unsigned long long currentKernel = FileTimeToInt64(kernelTime);
    unsigned long long currentUser = FileTimeToInt64(userTime);

    if (firstCall){
        preIdle = currentIdle;
        preKernel = currentKernel;
        preUser = currentUser;
        firstCall = false;
        
        cout << "\nCPU Load: Calculating..." << endl;
        return;
    }

    unsigned long long deltaIdle = currentIdle - preIdle;
    unsigned long long deltaKernel = currentKernel - preKernel;
    unsigned long long deltaUser = currentUser - preUser;

    unsigned long long totalSystemTime = deltaKernel + deltaUser;

    if (totalSystemTime > 0) {
        double cpuLoad = (double)(totalSystemTime - deltaIdle) * 100.0 / totalSystemTime;
        renderCPUInfo(cpuLoad);
    } else {
        cout << "CPU Load: 0.00%" << endl;
    }

    preIdle = currentIdle;
    preKernel = currentKernel;
    preUser = currentUser;
}

void getDiskInfo(){
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

    if (GetDiskFreeSpaceExA("C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)){ 
        double total = (double)totalNumberOfBytes.QuadPart / GB;
        double free = (double)totalNumberOfFreeBytes.QuadPart / GB;
        double used = total - free;
        double usedPercentage = (used / total) * 100.0;

        renderDiskInfo(usedPercentage, used, total);
    } else {
        cerr << "Error: could not get disk info. Code: " << GetLastError() << endl;
    }
}

void getUptimeInfo(){
    ULONGLONG totalSeconds = GetTickCount64() / 1000;
    
    ULONGLONG seconds = totalSeconds % 60;
    ULONGLONG minutes = (totalSeconds / 60) % 60;
    ULONGLONG hours = (totalSeconds / 3600) % 24;
    ULONGLONG days = totalSeconds / (3600 * 24);

    renderUptimeInfo(days, hours, minutes, seconds);
}

void getProcessesInfo(){
    DWORD processIds[1024];
    DWORD bytesReturned;

    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)){
        cout << "Error to enumerate processes" << endl;
        return; 
    }

    int count = bytesReturned / sizeof(DWORD);

    vector<DWORD> topPids;
    int limit = (count > 5) ? 5 : count;

    for (int i = 0; i < limit; i++) {
        topPids.push_back(processIds[i]);
    }

    renderProcessesInfo(count, topPids);
}

unsigned long long FileTimeToInt64(const FILETIME& ft){
    return (((unsigned long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}