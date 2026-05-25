#include <iostream>
#include <windows.h>
#include <vector>
#include <iomanip>

#include "PCinfo.h"
#include "consoleView.h"

using namespace std;

int main() {
    hideCursor();
    cout << fixed << setprecision(2);
    system("cls");

    while (true){
        setCursorPosition(0, 0);
        cout << "Logged in as: " << getUsernameInfo() << "                " << endl;

        getRAMInfo();
        getCPUInfo();
        getDiskInfo();
        getUptimeInfo();
        getProcessesInfo();

        Sleep(1000);
    }
    return 0;
}