#include <iostream>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <format>

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
    system("cls");

    string user = getUsernameInfo();

    while (true){
        setCursorPosition(0, 0);
        cout << format("{}Logged {}in as{}: {}\n\n", RESET, RED, RESET, user);

        getRAMInfo();
        getCPUInfo();
        getDiskInfo();
        getUptimeInfo();
        getProcessesInfo();

        Sleep(1000);
    }
    return 0;
}