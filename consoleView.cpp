#include "consoleView.h"
#include <windows.h>
#include <iostream> 
#include <iomanip>

using namespace std;

void setCursorPosition(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Скрываем
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void drawProgressBar(double percentage, int width){
    int filledWidth = (int)(percentage / 100 * width);

    string color = GREEN;
    if (percentage > 70) color = YELLOW;
    if (percentage > 90) color = RED;

    cout << "[";
    cout << color;
    for (int i = 0; i < width; i++){
        if (i < filledWidth) cout << "#";
        else cout << " ";
    }
    cout << RESET << "] " << fixed << setprecision(1) << percentage << "%   ";
}

void renderMemoryInfo(double load, double used, double total){
    cout << "┌── RAM " << string(32, '-') << RESET << endl;
    cout << "│  Usage: "; 
    drawProgressBar(load); 
    cout << endl;
    cout << "│  Detailed: " << used << " GB / " << total << " GB" << endl;
    cout << "└" << string(39, '-') << RESET << endl;
}

void renderCPUInfo(double percentage){
    cout << "┌── CPU " << string(32, '-') << RESET << endl;
    cout << "│  Usage: "; 
    drawProgressBar(percentage); 
    cout << endl;
    cout << "└" << string(39, '-') << RESET << endl;
}

void renderDiskInfo(double load, double used, double total){
    cout << "┌── Disk C " << string(30, '-') << RESET << endl;
    cout << "│  Usage: "; 
    drawProgressBar(load); 
    cout << endl;
    cout << "│  Detailed: " << used << " GB / " << total << " GB                 " << endl;
    cout << "└" << string(39, '-') << RESET << endl;
}

void renderUptimeInfo(unsigned long long days, unsigned long long hours, unsigned long long minutes, unsigned long long seconds){
    cout << "┌── CPU " << string(32, '-') << RESET << endl;
    cout << "│  PC work: ";
    if (days > 0) cout << days << " days, ";
    cout << hours << " hr " << minutes << " min " << seconds << " s                  " << endl;
    cout << "└" << string(39, '-') << RESET << endl;
}

// if (days > 0) cout << days << " days, ";