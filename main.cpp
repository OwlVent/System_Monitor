#include <iostream>
#include <windows.h>
#include <vector>
#include <iomanip>

#include "PCinfo.h"
#include "consoleView.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    hideCursor();
    cout << fixed << setprecision(2);
    system("cls");

    while (true){
        setCursorPosition(0, 0);
        cout << "Logged " << RED << "in as " << RESET << getUsernameInfo() << "                " << endl;
        cout << endl;

        getRAMInfo();
        getCPUInfo();
        getDiskInfo();
        getUptimeInfo();
        //getProcessesInfo();

        Sleep(1000);
    }
    return 0;
}