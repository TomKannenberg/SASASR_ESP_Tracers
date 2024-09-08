#pragma once
#include "libraries.hpp"
class CameraUtils {
public:
    CameraUtils(HANDLE processHandle, uintptr_t baseAddress);
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraTarget();
    float getFrom(uintptr_t offset);
    glm::vec3 getRacerPos(int index);

private:
    uint32_t camera;
    HANDLE processHandle;
    uintptr_t baseAddress;
};