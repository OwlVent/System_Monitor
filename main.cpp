#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <iomanip>

#include "PCinfo.h"

using namespace std;

int main() {
    cout << fixed << setprecision(2);

    while (true){
        cout << "Logged in as: " << getUsernameInfo() << endl;

        // ---- Блок RAM ----
        getRAMInfo();
        
        // ---- Блок CPU ----
        getCPUInfo();

        // ---- Блок DISK ----
        getDiskInfo();

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