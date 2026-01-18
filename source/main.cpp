#include <windows.h>
#include <tchar.h>
#include "Renderer.h"




// Globals
const int wWidth = 600;
const int wHeight = 500;
const TCHAR* WndClassName = TEXT("Window Blueprint");
const TCHAR* WndTitle = TEXT("Dxrend");

Renderer* g_renderer = nullptr;


// Forward Declaration
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


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

    // Initialize Renderer
    g_renderer = new Renderer();
    if(!g_renderer->Initialize(hWnd, wWidth, wHeight))
    {
        MessageBox(NULL, _T("Renderer Initialization failed!"), "Error", MB_ICONEXCLAMATION | MB_OK);
        delete g_renderer;
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

        // Render frame
        g_renderer->RenderFrame();
    }

    // Clean up
    g_renderer->Shutdown();
    delete g_renderer;

    return  msg.wParam;
}


// handle messages within the procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // Directx will paint, so no need to paint here
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
