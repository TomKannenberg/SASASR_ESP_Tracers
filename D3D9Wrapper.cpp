#include "D3D9Wrapper.hpp"

D3D9Overlay::D3D9Overlay(HWND targetWindow) : targetHWND(targetWindow) {
    try {
        createOverlayWindow();
        initD3D();
        setOverlayTransparent();
    } catch (const std::exception& e) {
        MessageBox(NULL, e.what(), "Initialization Error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
}

D3D9Overlay::~D3D9Overlay() {
    if (device) device->Release();
    if (d3d) d3d->Release();
    if (overlayHWND) DestroyWindow(overlayHWND);
}

void D3D9Overlay::initD3D() {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) {
        MessageBox(NULL, "Failed to create Direct3D9 object", "D3D Initialization Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to create Direct3D9 object");
    }

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = overlayHWND;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = 800;
    d3dpp.BackBufferHeight = 600;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlayHWND, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device);
    if (FAILED(hr)) {
        MessageBox(NULL, ("Failed to create Direct3D9 device. Error code: " + std::to_string(hr)).c_str(), "D3D Initialization Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to create Direct3D9 device");
    }
}

void D3D9Overlay::createOverlayWindow() {
    RECT targetRect;
    if (!GetWindowRect(targetHWND, &targetRect)) {
        MessageBox(NULL, ("Failed to get target window rect. Error code: " + std::to_string(GetLastError())).c_str(), "Window Creation Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to get target window rect");
    }
    int width = targetRect.right - targetRect.left;
    int height = targetRect.bottom - targetRect.top;

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "D3D9OverlayClass", NULL };
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, ("Failed to register window class. Error code: " + std::to_string(GetLastError())).c_str(), "Window Creation Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to register window class");
    }

    overlayHWND = CreateWindow(wc.lpszClassName, "D3D9 Overlay", WS_POPUP, targetRect.left, targetRect.top, width, height, NULL, NULL, wc.hInstance, this);
    if (!overlayHWND) {
        MessageBox(NULL, ("Failed to create overlay window. Error code: " + std::to_string(GetLastError())).c_str(), "Window Creation Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to create overlay window");
    }

    if (!SetWindowPos(overlayHWND, HWND_TOPMOST, targetRect.left, targetRect.top, width, height, SWP_NOACTIVATE | SWP_NOSIZE)) {
        MessageBox(NULL, ("Failed to set window position. Error code: " + std::to_string(GetLastError())).c_str(), "Window Creation Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to set window position");
    }

    ShowWindow(overlayHWND, SW_SHOW);
    UpdateWindow(overlayHWND);
}


void D3D9Overlay::setOverlayTransparent() {
    LONG style = GetWindowLong(overlayHWND, GWL_EXSTYLE);
    SetWindowLong(overlayHWND, GWL_EXSTYLE, style | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);

    if (!SetLayeredWindowAttributes(overlayHWND, 0, 0, LWA_COLORKEY)) {
        MessageBox(NULL, ("Failed to set layered window attributes. Error code: " + std::to_string(GetLastError())).c_str(), "Window Creation Error", MB_OK | MB_ICONERROR);
        throw std::runtime_error("Failed to set layered window attributes");
    }
}

void D3D9Overlay::render() {
    if (!device) return;

    device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    device->BeginScene();

    ID3DXLine* pLine;
    HRESULT hr = D3DXCreateLine(device, &pLine);
    if (FAILED(hr)) {
        MessageBox(NULL, ("Failed to create line interface. Error code: " + std::to_string(hr)).c_str(), "Rendering Error", MB_OK | MB_ICONERROR);
        return;
    }

    RECT rc;
    GetClientRect(overlayHWND, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    width = 800;
    height = 600;

    constexpr glm::vec2 nv2 = glm::vec2(-1, -1);

    D3DXVECTOR2 linePoints[2];
    for (int i = 0; i < pos.size(); ++i) {
        if (i != 0 && pos[i] == nv2) {
            continue;
        }
        linePoints[0] = D3DXVECTOR2(pos[0].x * width, pos[0].y * height);
        linePoints[1] = D3DXVECTOR2(pos[i].x * width, pos[i].y * height);

        pLine->SetWidth(2.0f);
        pLine->Begin();
        pLine->SetWidth(1);
        pLine->Draw(linePoints, 2, D3DCOLOR_XRGB(255, 128, 255));
        pLine->End();
    }

    pLine->Release();

    device->EndScene();
    device->Present(NULL, NULL, NULL, NULL);
}


LRESULT CALLBACK D3D9Overlay::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void D3D9Overlay::renderLoop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        render();
        Sleep(16);
    }
}

void D3D9Overlay::setPos(const glm::vec2& v, int index) {
    this->pos[index] = v;
}
