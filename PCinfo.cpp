#include "PCinfo.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <iomanip>

using namespace std;

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
        const float GB = 1024 * 1024 * 1024.0f;

        cout << "\n------Memory: Information------" << endl;
        cout << "Memory Load: " << memInfo.dwMemoryLoad << "%" << endl;
        cout << "Total Physical RAM: " << memInfo.ullTotalPhys / GB << " GB" << endl;
        cout << "Free Physical RAM: " << memInfo.ullAvailPhys / GB << " GB" << endl;
        cout << "Used Physical RAM: " << (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / GB << " GB" << endl;
    } else {
        cerr << "Error getting memory status. Error code: " << GetLastError() << endl;
    }
}

void getCPUInfo(){
    FILETIME idleTime, kernelTime, userTime;
        
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    unsigned long long preIdle = FileTimeToInt64(idleTime);   
    unsigned long long preKernel = FileTimeToInt64(kernelTime);
    unsigned long long preUser = FileTimeToInt64(userTime);
    
    Sleep(1000);

    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    unsigned long long postIdle = FileTimeToInt64(idleTime);   
    unsigned long long postKernel = FileTimeToInt64(kernelTime);
    unsigned long long postUser = FileTimeToInt64(userTime);

    unsigned long long deltaIdle = postIdle - preIdle;
    unsigned long long deltaKernel = postKernel - preKernel;
    unsigned long long deltaUser = postUser - preUser;

    unsigned long long totalSystemTime = deltaKernel + deltaUser;

    if (totalSystemTime > 0) {
        cout << "\n------CPU: Information------" << endl;
        double cpuLoad = (double)(totalSystemTime - deltaIdle) * 100.0 / totalSystemTime;
        cout << "\nCPU Load: " << cpuLoad << "%" << endl;
    } else {
        cerr << "Error: could not get CPU info. Code: " << GetLastError() << endl;
    }
}

unsigned long long FileTimeToInt64(const FILETIME& ft){
    return (((unsigned long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}