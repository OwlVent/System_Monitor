#pragma once
#include <string>
#include <vector>
#include <windows.h>

inline const std::string RESET = "\033[0m";
inline const std::string RED   = "\033[31m";
inline const std::string GREEN = "\033[32m";
inline const std::string YELLOW= "\033[33m";
inline const std::string BLUE  = "\033[34m";
inline const std::string CYAN  = "\033[36m";

void setCursorPosition(int x, int y);
void hideCursor();
void drawProgressBar(double percentage, int width = 20);
void renderMemoryInfo(double load, double used, double total);
void renderCPUInfo(double percentage);
void renderDiskInfo(double load, double used, double total);
void renderUptimeInfo(unsigned long long days, unsigned long long hours, unsigned long long minutes, unsigned long long seconds);
void renderProcessesInfo(int count, const std::vector<DWORD>& topPids);