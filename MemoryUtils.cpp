#include "MemoryUtils.hpp"
#include <iostream>

std::vector<uint8_t> MemoryUtils::readMemoryFromProcess(HANDLE processHandle, uintptr_t baseAddress, uintptr_t relativeAddress, SIZE_T size) {
    uintptr_t absoluteAddress = baseAddress + relativeAddress;
    std::vector<uint8_t> buffer(size);

    SIZE_T bytesRead;
    if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(absoluteAddress), buffer.data(), size, &bytesRead)) {
        if (bytesRead == size) {
            return buffer;
        } else {
            printf("Failed to read the full value.\n");
        }
    } else {
        printf("ReadProcessMemory failed.\n");
    }

    return std::vector<uint8_t>();
}

void MemoryUtils::writeMemoryToProcess(HANDLE processHandle, uintptr_t baseAddress, uintptr_t relativeAddress, uint8_t* value, size_t datasize) {
    uintptr_t absoluteAddress = baseAddress + relativeAddress;

    SIZE_T bytesWritten;
    if (WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(absoluteAddress), value, datasize, &bytesWritten)) {
        if (bytesWritten == datasize) {
        } else {
            printf("Failed to write the full value.\n");
        }
    } else {
        printf("WriteProcessMemory failed.\n");
    }
}
