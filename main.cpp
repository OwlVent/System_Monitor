#include <iostream>
#include <windows.h>
#include <vector>
#include <iomanip>

#include "PCinfo.h"

using namespace std;

int main() {
    cout << fixed << setprecision(2);

    while (true){
        cout << "Logged in as: " << getUsernameInfo() << endl;

        getRAMInfo();
        getCPUInfo();
        getDiskInfo();
        getUptimeInfo();
        getProcessesInfo();

        Sleep(1000);
        system("cls");
    }
    return 0;
}