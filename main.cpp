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
        getUptimeInfo();

        // ---- Блок PROCESSES ---- 
        getProcessesInfo();

        Sleep(2000);
        system("cls");
    }
    return 0;
}