#pragma once
#include "libraries.hpp"

class ProcessUtils {
public:
    static DWORD findProcessId(const char* processName);
    static HWND GetHwndFromProcessId(DWORD processId);
    static uintptr_t findModuleBaseAddress(HANDLE processHandle, const char* moduleName);

private:
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
};
