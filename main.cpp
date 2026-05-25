#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <iomanip>

using namespace std;

unsigned long long FileTimeToInt64(const FILETIME& ft){
    return (((unsigned long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

int main() {
    DWORD size = 0;
    GetUserNameA(NULL, &size);
    string name(size, '\0');
    GetUserNameA(&name[0], &size);
    if (!name.empty()) name.resize(size - 1);

    cout << fixed << setprecision(2);

    while (true){
        cout << "Logged in as: " << name << endl;

        // ---- Блок RAM ----
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
        
        // ---- Блок CPU ----
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

        // ---- Блок DISK ----
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

        if (GetDiskFreeSpaceExA("C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)){ 
            const float GB = 1024 * 1024 * 1024.0f;

            cout << "\n------Disk C: Information------" << endl;
            cout << "Total size: " << totalNumberOfBytes.QuadPart / GB << "GB" << endl;
            cout << "Free space: " << totalNumberOfFreeBytes.QuadPart / GB << "GB" << endl;
            cout << "Available for you: " << freeBytesAvailable.QuadPart / GB << "GB" << endl;

            double usedPercentage = 100.0 - (double)totalNumberOfFreeBytes.QuadPart * 100.0 / totalNumberOfBytes.QuadPart;
            cout << "Disk usage: " << usedPercentage << "%" << endl;
        } else {
            cerr << "Error: could not get disk info. Code: " << GetLastError() << endl;
        }

        // ---- Блок UPTIME ----
        ULONGLONG uptime = GetTickCount64() / 1000;
        ULONGLONG days = uptime / 3600 / 24;
        cout << "\n------ Uptime ------" << endl;
        if (days > 0) cout << days << " days, ";
        cout << uptime / 3600 % 24 << "h " << (uptime % 3600) / 60 << "m " << uptime % 60 << "s" << endl;

        // ---- Блок PROCESSES ---- 
        cout << "\n------Processes: Information------" << endl;
        DWORD processIds[1024];
        DWORD bytesReturned;

        if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)){
            cout << "Error to enumerate processes" << endl;
            return 1;
        }

        int count = bytesReturned / sizeof(DWORD);

        cout << "Found " << count << " running processes" << endl;

        for (int i = 0; i < 10 && i < count; i++){
            cout << "Processes ID: " << processIds[i] << endl;
        }

        Sleep(2000);
        system("cls");
    }
    return 0;
}