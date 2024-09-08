#include "ProcessUtils.hpp"
#include "MemoryUtils.hpp"
#include "CameraUtils.hpp"
#include "D3D9Wrapper.hpp"
#include "GUtils.hpp"

using namespace glm;

glm::vec2 WorldToScreen(const glm::vec3& targetPos, const glm::mat4& viewM, const glm::mat4& projM) {

    if (targetPos == vec3(-1)) {
        return vec2(-1, -1);
    }

    //(viewM, "View Matrix:");
    //pM(projM, "Proj Matrix:");

    vec4 viewSpacePos = viewM * vec4(targetPos, 1.0f);

    if (-viewSpacePos.z < 0.0f) {
        return vec2(-1, -1);
    }

    vec4 clipSpacePos = projM * viewSpacePos;

    if (fabs(clipSpacePos.w) < 1e-6) {
        return vec2(-1, -1);
    }

    vec3 ndcPos = vec3(clipSpacePos) / clipSpacePos.w;

    vec2 screenPos;
    screenPos.x = (ndcPos.x * 0.5f + 0.5f);
    screenPos.y = (1.0f - (ndcPos.y * 0.5f + 0.5f));

    return screenPos;
}


int main() {

    const char* moduleName = "Sonic & SEGA All-Stars Racing.exe";
    DWORD processId = ProcessUtils::findProcessId(moduleName);
    HWND sonicwindow = FindWindow(NULL, "Sonic & SEGA All-Stars Racing");
    if (!sonicwindow) {
        MessageBox(NULL, "Failed to find target window", "Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    if (processId == 0) {
        std::cerr << "Process not found.\n";
        while (1);
    }

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle == NULL) {
        std::cerr << "Failed to open process.\n";
        while (1);
    }

    uintptr_t baseAddress = ProcessUtils::findModuleBaseAddress(processHandle, moduleName);
    if (baseAddress == 0) {
        std::cerr << "Module not found.\n";
        CloseHandle(processHandle);
        while (1);
    } else {
        //std::cout << "Module found!\n";
    }

    CameraUtils cameraUtils(processHandle, baseAddress);

    bool enabled = false;

    D3D9Overlay overlay(sonicwindow);

    std::thread renderThread(&D3D9Overlay::renderLoop, &overlay);

    int a = 0;
    while (true) {
        vec3 CameraPos = cameraUtils.getCameraPosition();
        vec3 TargetPos = cameraUtils.getCameraTarget();

        {
            
            cls();

            mat4x4 viewM;
            mat4x4 projM;

            int index = 0;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    viewM[i][j] = cameraUtils.getFrom(0x0 + index);
                    index += 4;
                }
            }

            index = 0;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    projM[i][j] = cameraUtils.getFrom(0x40 + index);
                    index += 4;
                }
            }

            pv(CameraPos, "Camera Pos:");
            pv(TargetPos, "Target Pos:");

            //overlay.setPos(WorldToScreen(TargetPos, viewM, projM), 7);

            for (int i = 0; i < 8; i++) {
                //std::string n = "Racer ";
                //n += std::to_string(i + 1);
                //n += "'s Pos";

                //pv(cameraUtils.getRacerPos(i), n);

                overlay.setPos(WorldToScreen(cameraUtils.getRacerPos(i), viewM, projM), i);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    CloseHandle(processHandle);

    return 0;
}