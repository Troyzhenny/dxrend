#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


class Renderer
{
    public:
        Renderer();
        ~Renderer();

        bool Initialize(HWND hWnd, int wWidth, int wHeight);

        void RenderFrame();
        void Shutdown();

    private:
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_deviceContext = nullptr;
        IDXGISwapChain* m_swapChain = nullptr;
        ID3D11RenderTargetView* m_renderTargetView = nullptr;

        ID3D11VertexShader* m_vertexShader = nullptr;
        ID3D11PixelShader* m_pixelShader = nullptr;
        ID3D11InputLayout* m_inputLayout = nullptr;
        ID3D11Buffer* m_vertexBuffer = nullptr;

        // Helpers
        bool InitializeShaders();
        bool InitializeTriangle();
};
