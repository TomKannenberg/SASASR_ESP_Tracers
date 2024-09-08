#include "libraries.hpp"

class D3D9Overlay {
public:
    D3D9Overlay(HWND targetWindow);
    ~D3D9Overlay();

    void renderLoop();
    void setPos(const glm::vec2 &v, int index = 0);

private:
    HWND targetHWND;
    HWND overlayHWND;
    LPDIRECT3D9 d3d = nullptr;
    LPDIRECT3DDEVICE9 device = nullptr;

    void initD3D();
    void createOverlayWindow();
    void setOverlayTransparent();
    void render();

    std::vector<glm::vec2> pos = std::vector<glm::vec2>(8);

    void restoreOverlayWindowSettings(HWND hwnd, RECT originalRect);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};