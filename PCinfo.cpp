#include "PCinfo.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <iomanip>

using namespace std;

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