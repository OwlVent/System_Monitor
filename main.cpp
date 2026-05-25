#include <iostream>
#include <windows.h>

using namespace std;

unsigned long long FileTimeToInt64(const FILETIME& ft){
    return (((unsigned long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

int main() {
    while (true){
        // Используем структуру MEMORYSTATUSEX для получения информации о состоянии памяти
        MEMORYSTATUSEX memInfo;

        // Установка размера структуры
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);

        if (GlobalMemoryStatusEx(&memInfo)) {
            // Константа для перевода байтов в гигабайты
            const float GB = 1024 * 1024 * 1024.0f;

            cout << "Memory Load: " << memInfo.dwMemoryLoad << "%" << endl;
            cout << "Total Physical RAM: " << memInfo.ullTotalPhys / GB << " GB" << endl;
            cout << "Free Physical RAM: " << memInfo.ullAvailPhys / GB << " GB" << endl;
            cout << "Used Physical RAM: " << (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / GB << " GB" << endl;
        } else {
            cerr << "Error getting memory status. Error code: " << GetLastError() << endl;
        }
        
        // Используем GetSystemTimes для получения информации о заполненности CPU
        FILETIME idleTime, kernelTime, userTime;
        
        GetSystemTimes(&idleTime, &kernelTime, &userTime);

        unsigned long long preIdle = FileTimeToInt64(idleTime);   
        unsigned long long preKernel = FileTimeToInt64(kernelTime);
        unsigned long long preUser = FileTimeToInt64(userTime);
        
        Sleep(3000);

        GetSystemTimes(&idleTime, &kernelTime, &userTime);

        unsigned long long postIdle = FileTimeToInt64(idleTime);   
        unsigned long long postKernel = FileTimeToInt64(kernelTime);
        unsigned long long postUser = FileTimeToInt64(userTime);

        unsigned long long deltaIdle = postIdle - preIdle;
        unsigned long long deltaKernel = postKernel - preKernel;
        unsigned long long deltaUser = postUser - preUser;

        unsigned long long totalSystemTime = deltaKernel + deltaUser;

        if (totalSystemTime > 0) {
            double cpuLoad = (double)(totalSystemTime - deltaIdle) * 100.0 / totalSystemTime;
            cout << "\nCPU Load: " << cpuLoad << "%" << endl;
        }

        Sleep(5000);
        system("cls");
    }
    return 0;
}