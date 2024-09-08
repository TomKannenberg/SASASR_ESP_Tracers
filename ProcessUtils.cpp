#include "ProcessUtils.hpp"
#include <tlhelp32.h>
#include <psapi.h>
#include <cstring>

DWORD ProcessUtils::findProcessId(const char* processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (strstr(processEntry.szExeFile, processName)) {
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return 0;
}

BOOL CALLBACK ProcessUtils::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    if (processId == lParam) {
        *(HWND*)lParam = hwnd;
        return FALSE;
    }

    return TRUE;
}

HWND ProcessUtils::GetHwndFromProcessId(DWORD processId) {
    HWND g_foundHwnd = NULL;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&g_foundHwnd));
    return g_foundHwnd;
}

uintptr_t ProcessUtils::findModuleBaseAddress(HANDLE processHandle, const char* moduleName) {
    HMODULE modules[1024];
    DWORD bytesNeeded;

    if (EnumProcessModules(processHandle, modules, sizeof(modules), &bytesNeeded)) {
        for (size_t i = 0; i < (bytesNeeded / sizeof(HMODULE)); ++i) {
            char moduleFileName[MAX_PATH];
            if (GetModuleFileNameEx(processHandle, modules[i], moduleFileName, sizeof(moduleFileName))) {
                if (strstr(moduleFileName, moduleName)) {
                    return reinterpret_cast<uintptr_t>(modules[i]);
                }
            }
        }
    }
    return 0;
}
