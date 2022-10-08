#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cmath>

#include "graphics.h"
#include "d3ddef.h"
#include "common.h"

#include "bindable/indexbuffer.h"
#include "bindable/constantbuffer.h"
#include "bindable/vertexbuffer.h"
#include "bindable/topology.h"
#include "bindable/inputlayout.h"

class Graphics::Private 
{
public:
    Private(Graphics& gfx, HWND hwnd)
        : graphics { gfx }
        , hWnd{ hwnd }
    {
        initD3d();
    }

    void initD3d() 
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = 1920;
        sd.BufferDesc.Height = 1080;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = hWnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        HR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
            nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd,
            swapChain.ReleaseAndGetAddressOf(),
            device.ReleaseAndGetAddressOf(),
            nullptr, ctx.ReleaseAndGetAddressOf()));

        ComPtr<ID3D11Resource> buffer;
        HR(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), 
            reinterpret_cast<void**>(buffer.ReleaseAndGetAddressOf())));
        HR(device->CreateRenderTargetView(buffer.Get(), nullptr, targetView.ReleaseAndGetAddressOf()));
    }

    void clearBuffer(float red, float green, float blue)
    {
        const float color[] = { red, green, blue, 1.0f };
        ctx->ClearRenderTargetView(targetView.Get(), color);
    }

    void drawTriangle() 
    {
        struct Vertex
        {
            float x;
            float y;
            std::uint8_t r;
            std::uint8_t g;
            std::uint8_t b;
            std::uint8_t a;
        };

        // 创建顶点缓冲
        std::vector<Vertex> vertexs = {
            { 0.0f, 0.5f, 255, 0, 0, 0 },
            { 0.5f, -0.5f, 0, 255, 0, 0 },
            { -0.5f, -0.5f, 0, 0, 255, 0 },

            { -0.3f, 0.3f, 0, 255, 0, 0 },
            { 0.3f, 0.3f, 0, 0, 255, 0 },
            { 0.0f, -0.8f, 255, 0, 0, 0 },
        };

        VertexBuffer{ graphics, vertexs }.bind();
        
        // create index buffer
        std::vector<std::uint16_t> indices = {
            0, 1, 2,
            0, 2, 3,
            0, 4, 1,
            2, 1, 5,
        };

        IndexBuffer{ graphics, indices }.bind();
        
        // create constant buffer
        struct ConstantBuffer 
        {
            struct
            {
                float element[4][4];
            }transformation;
        };
        
        static float angle = 0;
        angle+=0.1f;
        if (angle > 100)
            angle = 0;
        const ConstantBuffer cb = {
            {
                (9.0f / 16.0f)* std::cos(angle), std::sin(angle), 0.0f, 0.0f,
                (9.0f / 16.0f) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0, 0.0f, 1.0f,
            }
        };
        VertexConstantBuffer<ConstantBuffer>{ graphics, cb }.bind();
        
        ComPtr<ID3DBlob> blob;

        // 创建顶点着色器
        ComPtr<ID3D11VertexShader> vertexShader;
        std::wstring fileName = GetApplicationDirW() + L"\\vertexshader.cso";
        HR(D3DReadFileToBlob(fileName.c_str(), blob.ReleaseAndGetAddressOf()));
        HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 
            nullptr, vertexShader.ReleaseAndGetAddressOf()));
        
        // bind 顶点着色器
        ctx->VSSetShader(vertexShader.Get(), nullptr, 0);

        // input layout
        std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
            { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        InputLayout{ graphics, ied, blob.Get()}.bind();

        // 创建像素着色器
        ComPtr<ID3D11PixelShader> pixelShader;
        fileName = GetApplicationDirW() + L"\\pixshader.cso";
        HR(D3DReadFileToBlob(fileName.c_str(), blob.ReleaseAndGetAddressOf()));
        HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(),
            nullptr, pixelShader.ReleaseAndGetAddressOf()));

        // bind 像素着色器
        ctx->PSSetShader(pixelShader.Get(), nullptr, 0);

        // bind render target
        ctx->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);

        Topology{ graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST }.bind();

        // configure viewport
        D3D11_VIEWPORT vp;
        vp.Width = 1920;
        vp.Height = 1080;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        ctx->RSSetViewports(1, &vp);

        ctx->DrawIndexed(static_cast<UINT>(std::size(indices)), 0, 0);
    }

    void render() 
    {
        clearBuffer(0.0f, 0.0f, 0.0f);
        drawTriangle();
        swapChain->Present(1, 0);
    }

public:
    Graphics& graphics;
    HWND hWnd;
    ComPtr<ID3D11Device> device;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11DeviceContext> ctx;
    ComPtr<ID3D11RenderTargetView> targetView;
    DirectX::XMMATRIX projection;
};

Graphics::Graphics(HWND hwnd)
    : MAKE_PRIVATRE(Graphics, *this, hwnd)
{
}

Graphics::~Graphics()
{
}

void Graphics::render()
{
    p_->render();
}

void Graphics::drawIndexed(std::size_t count)
{
    p_->ctx->DrawIndexed(static_cast<UINT>(count), 0u, 0u);
}

void Graphics::setProjection(DirectX::FXMMATRIX projection)
{
    p_->projection = projection;
}

DirectX::XMMATRIX Graphics::projection()
{
    return p_->projection;
}

ID3D11Device* Graphics::device()
{
    return p_->device.Get();
}

ID3D11DeviceContext* Graphics::deviceContext()
{
    return p_->ctx.Get();
}
