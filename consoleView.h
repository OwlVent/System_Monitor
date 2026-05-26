#pragma once
#include <string>

const std::string RESET = "\033[0m";
const std::string RED   = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW= "\033[33m";
const std::string BLUE  = "\033[34m";
const std::string CYAN  = "\033[36m";

void setCursorPosition(int x, int y);
void hideCursor();
void drawProgressBar(double percentage, int width = 20);
void renderMemoryInfo(double load, double used, double total);
void renderCPUInfo(double percentage);
void renderDiskInfo(double load, double used, double total);
void renderUptimeInfo(unsigned long long days, unsigned long long hours, unsigned long long minutes, unsigned long long seconds);