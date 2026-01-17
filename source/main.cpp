#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
// #include <string>
// #include "terminal_colors.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")



// Globals
const int wWidth = 600;
const int wHeight = 500;
const TCHAR* WndClassName = TEXT("Window Blueprint");
const TCHAR* WndTitle = TEXT("Dxrend");

// Forward Declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool InitD3D(HWND hWnd);
void CleanD3D();
void RenderFrame();

// DX globals
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_deviceContext = nullptr;
IDXGISwapChain* g_swapChain = nullptr;
ID3D11RenderTargetView* g_renderTargetView = nullptr;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0, 168, 95));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WndClassName;
    wc.hIcon = nullptr;


    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Done know it mash up!"), NULL);
        return 1;
    }

    HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, WndClassName, WndTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wWidth, wHeight, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        MessageBox(NULL, _T("Call to CreateWindowEx failed!"), _T("Windows Guided Tour"), NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Initialize DX
    if(!InitD3D(hWnd))
    {
        MessageBox(NULL, _T("DirectX Initialization failed!"), "Error", MB_ICONEXCLAMATION | MB_OK);
        CleanD3D();
        return 0;
    }


    MSG msg = {};
    bool running = true;
    while (running)
    {
        // Handle messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        RenderFrame();
    }

    // Clean up
    CleanD3D();

    return  msg.wParam;
}

// Create Swapchain + description
bool InitD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd ={};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = 0;
    scd.BufferDesc.Height = 0;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Device. Context, and Swap Chain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &g_swapChain,
        &g_device,
        NULL,
        &g_deviceContext
    );

    if(FAILED(hr))
    {
        return false;
    }

    // Get back buffer
    ID3D11Texture2D* backBuffer = nullptr;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &backBuffer);

    // Render target view creation
    g_device->CreateRenderTargetView(backBuffer, NULL, &g_renderTargetView);
    backBuffer->Release();

    // Set render target
    g_deviceContext->OMSetRenderTargets(1, &g_renderTargetView, NULL);

    // Set viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = wWidth;
    viewport.Height = wHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    g_deviceContext->RSSetViewports(1, &viewport);

    return true;
}

void RenderFrame()
{
    // Clear color
    float clearColor[4] = {1.0f, 0.522f, 0.349f, 0.961f};
    g_deviceContext->ClearRenderTargetView(g_renderTargetView, clearColor);

    // Swap front and back buffer
    g_swapChain->Present(0,0);
};

void CleanD3D()
{
    /** Original order from forward declarations
        g_device
        g_deviceContext
        g_swapChain
        g_renderTargetView
    **/
  // Release core objects in reverse order
  if (g_renderTargetView) g_renderTargetView->Release();
  if (g_swapChain) g_swapChain->Release();
  if (g_deviceContext) g_deviceContext->Release();
  if (g_device) g_device->Release();
};


// handle messages within the procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // Directx will paunt, so no need to paint here
            EndPaint(hWnd, &ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_KEYDOWN: // WM_CHAR is for text input WM_KEYDOWN is for key presses like WASD
            if (wParam == 'C')
            {
                SetWindowText(hWnd, "New Window Name");
            }
            break;

        default:
            return DefWindowProc(hWnd,message, wParam, lParam);
            break;
    }

    return 0;
}
