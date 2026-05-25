#include "PCinfo.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <iomanip>

using namespace std;

string getUsernameInfo(){
    DWORD size = 0;
    GetUserNameA(NULL, &size);
    if (size == 0) return "Unknow";

    string name(size, '\0');
    GetUserNameA(&name[0], &size);
    if (!name.empty()) name.resize(size - 1);

    return name;
}