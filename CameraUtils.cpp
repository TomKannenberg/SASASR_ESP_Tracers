#include "CameraUtils.hpp"
#include "MemoryUtils.hpp"
#include "GUtils.hpp"

CameraUtils::CameraUtils(HANDLE processHandle, uintptr_t baseAddress)
    : processHandle(processHandle), baseAddress(baseAddress) {}

glm::vec3 CameraUtils::getCameraPosition() {
    uintptr_t relativeAddress = 0x0082A100;
    uintptr_t absoluteAddress = baseAddress + relativeAddress;
    uint32_t camera = *reinterpret_cast<uint32_t*>(MemoryUtils::readMemoryFromProcess(processHandle, absoluteAddress, 0, sizeof(uint32_t)).data()) + 0x20;

    glm::vec3 position = {
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, camera, 0x80, 4).data()),
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, camera, 0x84, 4).data()),
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, camera, 0x88, 4).data())
    };
    return position;
}

glm::vec3 CameraUtils::getCameraTarget() {
    uintptr_t relativeAddress = 0x0082A100;
    uintptr_t absoluteAddress = baseAddress + relativeAddress;
    uint32_t camera = *reinterpret_cast<uint32_t*>(MemoryUtils::readMemoryFromProcess(processHandle, absoluteAddress, 0, sizeof(uint32_t)).data()) + 0x20;

    glm::vec3 target = {
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, camera, 0x90, 4).data()),
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, camera, 0x94, 4).data()),
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, camera, 0x98, 4).data())
    };
    return target;
}

float CameraUtils::getFrom(uintptr_t offset) {
    uintptr_t relativeAddress = 0x0082A100;
    uintptr_t absoluteAddress = baseAddress + relativeAddress;
    uint32_t camera = *reinterpret_cast<uint32_t*>(MemoryUtils::readMemoryFromProcess(processHandle, absoluteAddress, 0, sizeof(uint32_t)).data()) + 0x20;

    //p(std::to_string(absoluteAddress));

    return *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, camera, offset, 4).data());
}

glm::vec3 CameraUtils::getRacerPos(int index) {
    uintptr_t relativeAddress = 0x00130DFC;
    uintptr_t absoluteAddress = baseAddress + relativeAddress;
    uint32_t entityList = *reinterpret_cast<uint32_t*>(MemoryUtils::readMemoryFromProcess(processHandle, absoluteAddress, 0, sizeof(uint32_t)).data());

    uint32_t entity = *reinterpret_cast<uint32_t*>(MemoryUtils::readMemoryFromProcess(processHandle, entityList + index * 16, 0, sizeof(uint32_t)).data());

    std::cout << index << std::endl;

    if (entity == 0) {
        return glm::vec3(-1);
    }

    glm::vec3 pos = {
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, entity + 0xC0, 0, sizeof(uint32_t)).data()),
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, entity + 0xC4, 0, sizeof(uint32_t)).data()),
        *reinterpret_cast<float*>(MemoryUtils::readMemoryFromProcess(processHandle, entity + 0xC8, 0, sizeof(uint32_t)).data())
    };
    return pos;
}
