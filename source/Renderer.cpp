#include "Renderer.h"
#include <windows.h>


struct Vertex
{
  float x, y, z;
  float r, g, b, a;
};

Renderer::Renderer()
{
    m_device = nullptr;
    m_deviceContext = nullptr;
    m_swapChain = nullptr;
    m_renderTargetView = nullptr;
    m_vertexShader = nullptr;
    m_pixelShader = nullptr;
    m_inputLayout = nullptr;
    m_vertexBuffer = nullptr;

};

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize(HWND hWnd, int wWidth, int wHeight)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
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
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_deviceContext
    );

    if(FAILED(hr))
    {
        OutputDebugStringA("Failed to create device and swap chain\n");
        return false;
    }

    // Get back buffer
    ID3D11Texture2D* backBuffer = nullptr;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &backBuffer);

    // Render target view creation
    m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
    backBuffer->Release();

    // Set render target
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);

    // Set viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = wWidth;
    viewport.Height = wHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_deviceContext->RSSetViewports(1, &viewport);

    // Initialize shaders and geometry
    if (!InitializeShaders())
    {
        return false;
    }

    if (!InitializeTriangle())
    {
        return false;
    }

    return true;
}

bool Renderer::InitializeShaders()
{
    // Compile vertex shader
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompileFromFile(
      L"shaders/VertexShader.hlsl",
      nullptr,
      nullptr,
      "main",
      "vs_5_0",
      0,
      0,
      &vsBlob,
      &errorBlob
    );

    if (FAILED(hr))
    {
        if(errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        return false;
    }

    m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_vertexShader
    );

    // Compile pixel shader
    ID3DBlob* psBlob = nullptr;

    hr = D3DCompileFromFile(
        L"shaders/PixelShader.hlsl",
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        0,
        0,
        &psBlob,
        &errorBlob
    );

    if (FAILED(hr))
    {
        if(errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        vsBlob->Release();
        return false;
    }

    m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pixelShader
    );

    psBlob->Release();

    // Create Input layout - describes how vertex data is structured
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    m_device->CreateInputLayout(
        layout,
        2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_inputLayout
    );

    vsBlob->Release();

    // Setting shader input and layout
    m_deviceContext->IASetInputLayout(m_inputLayout);
    m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
    m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

    return true;
}


bool Renderer::InitializeTriangle()
{
    Vertex triangleVertices[] =
    {
        { 0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f },   // Top (red)
        { 0.45f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },  // Right (green)
        {-0.45f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }   // Left (blue)
    };

    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth = sizeof(triangleVertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = triangleVertices;

    HRESULT hr = m_device->CreateBuffer(&bd, &initData, &m_vertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }



    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

void Renderer::RenderFrame()
{
    // Clear color
    float clearColor[4] = {1.0f, 0.522f, 0.349f, 0.961f};
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);

    // Draw 3 vertices
    m_deviceContext->Draw(3, 0);

    // Swap front and back buffer
    m_swapChain->Present(0,0);
};

void Renderer::Shutdown()
{
  // Released in reverse order
  if (m_vertexBuffer) m_vertexBuffer->Release();
  if (m_inputLayout) m_inputLayout->Release();
  if (m_pixelShader) m_pixelShader->Release();
  if (m_vertexShader) m_vertexShader->Release();

  if (m_renderTargetView) m_renderTargetView->Release();
  if (m_swapChain) m_swapChain->Release();
  if (m_deviceContext) m_deviceContext->Release();
  if (m_device) m_device->Release();
  //-------------------------------------------------------------------
};
