#include <Windows.h>
#include <d3d11.h>

#include "graphics.h"
#include "d3ddef.h"

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

        // bind render target
        ctx->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);

        // configure viewport
        D3D11_VIEWPORT vp;
        vp.Width = 1920;
        vp.Height = 1080;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        ctx->RSSetViewports(1, &vp);
    }

    void clearBuffer(float red, float green, float blue)
    {
        const float color[] = { red, green, blue, 1.0f };
        ctx->ClearRenderTargetView(targetView.Get(), color);
    }

    void render() 
    {
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

void Graphics::clearBuffer(float red, float green, float blue)
{
    p_->clearBuffer(red, green, blue);
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
