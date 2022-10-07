#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cmath>

#include "graphics.h"
#include "d3ddef.h"
#include "common.h"

class Graphics::Private 
{
public:
    Private(HWND hwnd) 
        : hwnd_{ hwnd }
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
        sd.OutputWindow = hwnd_;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        HR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
            nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd,
            swapChain_.ReleaseAndGetAddressOf(),
            device_.ReleaseAndGetAddressOf(),
            nullptr, ctx_.ReleaseAndGetAddressOf()));

        ComPtr<ID3D11Resource> buffer;
        HR(swapChain_->GetBuffer(0, __uuidof(ID3D11Resource), 
            reinterpret_cast<void**>(buffer.ReleaseAndGetAddressOf())));
        HR(device_->CreateRenderTargetView(buffer.Get(), nullptr, targetView_.ReleaseAndGetAddressOf()));
    }

    void clearBuffer(float red, float green, float blue)
    {
        const float color[] = { red, green, blue, 1.0f };
        ctx_->ClearRenderTargetView(targetView_.Get(), color);
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
        const Vertex vertexs[] = {
            { 0, 0.5, 255, 0, 0, 0 },
            { 0.5, -0.5, 0, 255, 0, 0 },
            { -0.5, -0.5, 0, 0, 255, 0 },

            { -0.3, 0.3, 0, 255, 0, 0 },
            { 0.3, 0.3, 0, 0, 255, 0 },
            { 0.0, -0.8, 255, 0, 0, 0 },
        };

        ComPtr<ID3D11Buffer> vertexBuffer;
        D3D11_BUFFER_DESC bd = {};
        bd.ByteWidth = sizeof(vertexs);
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        bd.StructureByteStride = sizeof(Vertex);

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertexs;
        HR(device_->CreateBuffer(&bd, &sd, vertexBuffer.ReleaseAndGetAddressOf()));

        // create index buffer
        const std::uint16_t indices[] = {
            0, 1, 2,
            0, 2, 3,
            0, 4, 1,
            2, 1, 5,
        };

        ComPtr<ID3D11Buffer> indexBuffer;
        D3D11_BUFFER_DESC ibd = {};
        ibd.ByteWidth = sizeof(indices);
        ibd.Usage = D3D11_USAGE_DEFAULT;
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        ibd.MiscFlags = 0;
        ibd.StructureByteStride = sizeof(std::uint16_t);

        D3D11_SUBRESOURCE_DATA isd = {};
        isd.pSysMem = indices;
        HR(device_->CreateBuffer(&ibd, &isd, indexBuffer.ReleaseAndGetAddressOf()));
        ctx_->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        // create constant buffer
        struct ConstantBuffer 
        {
            struct
            {
                float element[4][4];
            }transformation;
        };
        
        static float angle = 0;
        angle+=0.1;
        if (angle > 100)
            angle = 0;
        const ConstantBuffer cb = {
            {
                (9.0 / 16.0)* std::cos(angle), std::sin(angle), 0.0f, 0.0f,
                (9.0 / 16.0) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0, 0.0f, 1.0f,
            }
        };
        ComPtr<ID3D11Buffer> constBuffer;
        D3D11_BUFFER_DESC cbd = {};
        cbd.ByteWidth = sizeof(cb);
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0;
        cbd.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA csd = {};
        csd.pSysMem = &cb;
        HR(device_->CreateBuffer(&cbd, &csd, constBuffer.ReleaseAndGetAddressOf()));
        ctx_->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());


        // 添加顶点缓冲到pipline
        const UINT stride = sizeof(Vertex);
        const UINT offset = 0;
        ctx_->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

        ComPtr<ID3DBlob> blob;

        // 创建顶点着色器
        ComPtr<ID3D11VertexShader> vertexShader;
        std::wstring fileName = GetApplicationDirW() + L"\\vertexshader.cso";
        HR(D3DReadFileToBlob(fileName.c_str(), blob.ReleaseAndGetAddressOf()));
        HR(device_->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 
            nullptr, vertexShader.ReleaseAndGetAddressOf()));
        
        // bind 顶点着色器
        ctx_->VSSetShader(vertexShader.Get(), nullptr, 0);

        // input layout
        ComPtr<ID3D11InputLayout> inputLayout;
        const D3D11_INPUT_ELEMENT_DESC ied[] = {
            { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        HR(device_->CreateInputLayout(ied, std::size(ied), blob->GetBufferPointer(),
            blob->GetBufferSize(), inputLayout.ReleaseAndGetAddressOf()));
        ctx_->IASetInputLayout(inputLayout.Get());

        // 创建像素着色器
        ComPtr<ID3D11PixelShader> pixelShader;
        fileName = GetApplicationDirW() + L"\\pixshader.cso";
        HR(D3DReadFileToBlob(fileName.c_str(), blob.ReleaseAndGetAddressOf()));
        HR(device_->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(),
            nullptr, pixelShader.ReleaseAndGetAddressOf()));

        // bind 像素着色器
        ctx_->PSSetShader(pixelShader.Get(), nullptr, 0);

        // bind render target
        ctx_->OMSetRenderTargets(1, targetView_.GetAddressOf(), nullptr);

        //
        ctx_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // configure viewport
        D3D11_VIEWPORT vp;
        vp.Width = 1920;
        vp.Height = 1080;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        ctx_->RSSetViewports(1, &vp);

        ctx_->DrawIndexed(std::size(indices), 0, 0);
    }

    void render() 
    {
        clearBuffer(0.0f, 0.0f, 0.0f);
        drawTriangle();
        swapChain_->Present(1, 0);
    }

private:
    HWND hwnd_;
    ComPtr<ID3D11Device> device_;
    ComPtr<IDXGISwapChain> swapChain_;
    ComPtr<ID3D11DeviceContext> ctx_;
    ComPtr<ID3D11RenderTargetView> targetView_;
};

Graphics::Graphics(HWND hwnd)
    : MAKE_PRIVATRE(Graphics, hwnd)
{
}

Graphics::~Graphics()
{
}

void Graphics::render()
{
    p_->render();
}
