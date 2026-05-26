#include "consoleView.h"
#include <windows.h>
#include <iostream> 
#include <iomanip>
#include <format>

using namespace std;

void renderBodyRAMDisk(double load, double used, double total);

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
    cout << format("{}┌── RAM {:-<30}\n│  {}Usage: ", CYAN, "", RESET);
    renderBodyRAMDisk(load, used, total);
}

void renderCPUInfo(double percentage){
    cout << format("\n{}┌── CPU {:-<30}\n│  {}Usage: ", CYAN, "", RESET);
    drawProgressBar(percentage);
    cout << format("\n{}└{:-<20}", CYAN, "-");
}

void renderDiskInfo(double load, double used, double total){
    cout << format("\n{}┌── Disk {:-<29}\n│  {}Usage: ", CYAN, "", RESET);
    renderBodyRAMDisk(load, used, total);
}

void renderUptimeInfo(unsigned long long days, unsigned long long hours, unsigned long long minutes, unsigned long long seconds){
    cout << format("\n{}┌── Uptime {:-<27}\n│  {}PC work: ", CYAN, "", RESET);
    if (days > 0) cout << format("{} days, ",days);
    cout << format("{} hr {} min {} s{:<15}", hours, minutes, seconds, "");
    cout << format("\n{}└{:-<37}", CYAN, "-");
}

void renderBodyRAMDisk(double load, double used, double total){
    drawProgressBar(load); 
    cout << format("{}\n│  {}Detailed: {:.2f} GB / {:.2f} GB{:<10}\n{}└{:-<37}", CYAN, RESET, used, total, "", CYAN, "");
}