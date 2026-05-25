#include "PCinfo.h"
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
        
        cout << "\n------CPU: Information------" << endl;
        cout << "CPU Load: Calculating..." << endl;
        return;
    }

    unsigned long long deltaIdle = currentIdle - preIdle;
    unsigned long long deltaKernel = currentKernel - preKernel;
    unsigned long long deltaUser = currentUser - preUser;

    unsigned long long totalSystemTime = deltaKernel + deltaUser;

    cout << "\n------CPU: Information------" << endl;
    if (totalSystemTime > 0) {
        double cpuLoad = (double)(totalSystemTime - deltaIdle) * 100.0 / totalSystemTime;
        cout << "CPU Load: " << cpuLoad << "%                " << endl;
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
        cout << "\n------Disk C: Information------" << endl;
        cout << "Total size: " << totalNumberOfBytes.QuadPart / GB << "GB" << endl;
        cout << "Free space: " << totalNumberOfFreeBytes.QuadPart / GB << "GB" << endl;
        cout << "Available for you: " << freeBytesAvailable.QuadPart / GB << "GB" << endl;

        double usedPercentage = 100.0 - (double)totalNumberOfFreeBytes.QuadPart * 100.0 / totalNumberOfBytes.QuadPart;
        cout << "Disk usage: " << usedPercentage << "%" << endl;
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

    cout << "\n------ Uptime ------" << endl;
    if (days > 0) cout << days << " days, ";
    cout << hours << "h " << minutes << "m " << seconds << "s" << endl;
}

void getProcessesInfo(){
    cout << "\n------Processes: Information------" << endl;
    DWORD processIds[1024];
    DWORD bytesReturned;

    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)){
        cout << "Error to enumerate processes" << endl;
    }

    int count = bytesReturned / sizeof(DWORD);
    if (bytesReturned == sizeof(processIds)) {
        cout << " (Warning: Buffer full, maybe more processes exist)" << endl;
    }

    cout << "Found " << count << " running processes" << endl;

    for (int i = 0; i < 10 && i < count; i++){
        cout << "Processes ID: " << processIds[i] << endl;
    }
}

unsigned long long FileTimeToInt64(const FILETIME& ft){
    return (((unsigned long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}