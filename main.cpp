#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    // 1. Создаем структуру
    MEMORYSTATUSEX memInfo;

    // 2. Обязательно устанавливаем размер структуры
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    // 3. Вызываем функцию
    if (GlobalMemoryStatusEx(&memInfo)) {
        // Константа для перевода байтов в гигабайты
        const float GB = 1024 * 1024 * 1024.0f;

        cout << "Memory Load: " << memInfo.dwMemoryLoad << "%" << endl;
        cout << "Total Physical RAM: " << memInfo.ullTotalPhys / GB << " GB" << endl;
        cout << "Free Physical RAM: " << memInfo.ullAvailPhys / GB << " GB" << endl;
        cout << "Used Physical RAM: " << (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / GB << " GB" << endl;
    } else {
        cerr << "Error getting memory status. Error code: " << GetLastError() << endl;
    }
    
    return 0;
}