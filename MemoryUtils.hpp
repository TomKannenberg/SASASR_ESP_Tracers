#pragma once
#include "libraries.hpp"

class MemoryUtils {
public:
    static std::vector<uint8_t> readMemoryFromProcess(HANDLE processHandle, uintptr_t baseAddress, uintptr_t relativeAddress, SIZE_T size);
    static void writeMemoryToProcess(HANDLE processHandle, uintptr_t baseAddress, uintptr_t relativeAddress, uint8_t* value, size_t datasize = 1);
};